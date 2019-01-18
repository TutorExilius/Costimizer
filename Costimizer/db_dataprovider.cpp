#include "db_dataprovider.h"

#include <QDebug>

#include <QSqlError>
#include <QSqlQuery>

DB_DataProvider::DB_DataProvider( const QString &dbName )
{
    if( !QFile::exists(dbName) )
    {
        throw QString( "Error: DATABASE '" + dbName + "' not found" );
    }

    this->db = QSqlDatabase::addDatabase("QSQLITE");

    this->db.setDatabaseName( dbName );

    if( !db.open() )
    {
        throw QString("DB connecton failed");
    }
    else
    {
        qDebug() << "Database: connection ok";
    }

    this->db.open();
}

DB_DataProvider::~DB_DataProvider()
{
    this->db.close();
}

ShopItem DB_DataProvider::getShopItem( const QString &shopItemName ) const
{
    QSqlQuery query;

    if( query.exec("SELECT * FROM shopItem WHERE Name = '" + shopItemName + "'" ) )
    {
        if( query.next() )
        {
            const uint id = query.value(0).toUInt();
            const QString name = query.value(1).toString();

            return ShopItem{ id, name };
        }
    }
    else
    {
        qDebug() << "query.exec()-Error: " << this->db.lastError();
        return ShopItem{};
    }
}

ShopItem DB_DataProvider::getShopItem( const uint &shopItemID ) const
{
    QSqlQuery query;

    if( query.exec("SELECT * FROM shopItem WHERE ShopItemID = " + QString::number( shopItemID ) ) )
    {
        if( query.next() )
        {
            const uint id = query.value(0).toUInt();
            const QString name = query.value(1).toString();

            return ShopItem{ id, name };
        }
    }
    else
    {
        qDebug() << "query.exec()-Error: " << this->db.lastError();
        return ShopItem{};
    }
}

QString DB_DataProvider::getShopItemName( const uint &shopItemID  ) const
{
    QSqlQuery query;

    if( query.exec("SELECT * FROM shopItem WHERE ShopItemID = " + QString::number( shopItemID ) ) )
    {
        if( query.next() )
        {
            const QString name = query.value(1).toString();

            return name;
        }
    }
    else
    {
        qDebug() << "query.exec()-Error: " << this->db.lastError();
        return QString{};
    }
}

QList<ShopItem> DB_DataProvider::getShopItems() const
{
    QList<ShopItem> shopItems;

    QSqlQuery query;

    if( query.exec("SELECT * FROM shopItem") )
    {
        while( query.next() )
        {
            const uint id = query.value(0).toUInt();
            const QString name = query.value(1).toString();

            shopItems.append( ShopItem{ id, name } );
        }
    }
    else
    {
        qDebug() << "query.exec()-Error: " << this->db.lastError();
        return QList<ShopItem>{};
    }

    return shopItems;
}

Discounter DB_DataProvider::getDiscounter( const uint &discounterId ) const
{
    QSqlQuery query;

    if( query.exec("SELECT DiscounterID, Name, Location FROM discounter WHERE DiscounterID = " + QString::number( discounterId ) ) )
    {
        if( query.next() )
        {
            const uint id = query.value(0).toUInt();
            const QString name = query.value(1).toString();
            const QString location = query.value(2).toString();

            return Discounter{id, name, location};
        }
    }
    else
    {
        qDebug() << "query.exec()-Error: " << this->db.lastError();
        return Discounter{};
    }
}

QList<Discounter> DB_DataProvider::getDiscounters() const
{
    QList<Discounter> discounters;

    QSqlQuery query;

    if( query.exec("SELECT DiscounterID, Name, Location FROM discounter") )
    {
        while( query.next() )
        {
            const uint id = query.value(0).toUInt();
            const QString name = query.value(1).toString();
            const QString location = query.value(2).toString();

            discounters.append( Discounter{ id, name, location } );
        }
    }
    else
    {
        qDebug() << "query.exec()-Error: " << this->db.lastError();
        return QList<Discounter>{};
    }

    return discounters;
}

QList<DiscounterShopItem> DB_DataProvider::getLowPricedDiscounters( const uint &shopItemID ) const
{
    QList<DiscounterShopItem> discounterShopItems;

    QSqlQuery query;

    QString sql = "SELECT NormalPrice, ShopItemID, DiscounterID, OfferPrice ";
    sql += "FROM discounter_shopitem WHERE ShopItemID = " + QString::number( shopItemID ) + " " +
           "AND NormalPrice > 0.0 AND NormalPrice = (" +
            "SELECT MIN(NormalPrice) FROM discounter_shopitem WHERE ShopItemID = " +
            QString::number( shopItemID ) + " "  +
            "AND NormalPrice > 0.0)";

    if( query.exec( sql ) )
    {
        while( query.next() )
        {
            const uint shopItemID = query.value("ShopItemID").toUInt();
            const uint discounterID = query.value("DiscounterID").toUInt();
            const double normalPrice = query.value("NormalPrice").toDouble();
            const double offerPrice = query.value("OfferPrice").toDouble();

            discounterShopItems.append( DiscounterShopItem{ shopItemID, discounterID, normalPrice, offerPrice } );
        }
    }
    else
    {
        qDebug() << "query.exec()-Error: " << this->db.lastError();
        return QList<DiscounterShopItem>{};
    }

    return discounterShopItems;
}

QList<DiscounterShopItem> DB_DataProvider::getAllPricedDiscounters(const uint &shopItemID) const
{
    QList<DiscounterShopItem> discounterShopItems;

    QSqlQuery query;

    QString sql = "SELECT NormalPrice, ShopItemID, DiscounterID, OfferPrice ";
    sql += "FROM discounter_shopitem WHERE ShopItemID = " + QString::number( shopItemID ) + " " +
           "AND NormalPrice > 0.0";

    if( query.exec( sql ) )
    {
        while( query.next() )
        {
            const uint shopItemID = query.value("ShopItemID").toUInt();
            const uint discounterID = query.value("DiscounterID").toUInt();
            const double normalPrice = query.value("NormalPrice").toDouble();
            const double offerPrice = query.value("OfferPrice").toDouble();

            discounterShopItems.append( DiscounterShopItem{ shopItemID, discounterID, normalPrice, offerPrice } );
        }
    }
    else
    {
        qDebug() << "query.exec()-Error: " << this->db.lastError();
        return QList<DiscounterShopItem>{};
    }

    return discounterShopItems;
}


QList<DiscounterShopItem> DB_DataProvider::getDiscounterShopItems() const
{
    QList<DiscounterShopItem> discounterShopItems;

    QSqlQuery query;

    if( query.exec("SELECT ShopItemID, DiscounterID, NormalPrice, OfferPrice FROM discounter_shopitem") )
    {
        while( query.next() )
        {
            const uint shopItemID = query.value(0).toUInt();
            const uint discounterID = query.value(1).toUInt();
            const double normalPrice = query.value(2).toDouble();
            const double offerPrice = query.value(3).toDouble();

            discounterShopItems.append( DiscounterShopItem{ shopItemID, discounterID, normalPrice, offerPrice } );
        }
    }
    else
    {
        qDebug() << "query.exec()-Error: " << this->db.lastError();
        return QList<DiscounterShopItem>{};
    }

    return discounterShopItems;
}

QList<DiscounterShopItem> DB_DataProvider::getDiscounterShopItems( const uint &shopItemID ) const
{
    QList<DiscounterShopItem> discounterShopItems;

    QSqlQuery query;

    if( query.exec("SELECT ShopItemID, DiscounterID, NormalPrice, OfferPrice FROM discounter_shopitem WHERE ShopItemID = " + QString::number( shopItemID ) ) )
    {
        while( query.next() )
        {
            const uint shopItemID = query.value(0).toUInt();
            const uint discounterID = query.value(1).toUInt();
            const double normalPrice = query.value(2).toDouble();
            const double offerPrice = query.value(3).toDouble();

            discounterShopItems.append( DiscounterShopItem{ shopItemID, discounterID, normalPrice, offerPrice } );
        }
    }
    else
    {
        qDebug() << "query.exec()-Error: " << this->db.lastError();
        return QList<DiscounterShopItem>{};
    }

    return discounterShopItems;
}

double DB_DataProvider::getNormalPrice( const uint &shopItemID, const uint &discounterID ) const
{
    QSqlQuery query;

    const QString sql = "SELECT NormalPrice FROM discounter_shopitem WHERE ShopItemID = "
            + QString::number( shopItemID ) + " AND DiscounterID = "
            + QString::number( discounterID );

    if( query.exec( sql ) )
    {
        if( query.next() )
        {
            bool parseToDoubleSucceeded = false;
            double normalPrice = query.value( "NormalPrice" ).toDouble( &parseToDoubleSucceeded );

            if( parseToDoubleSucceeded )
            {
                return normalPrice;
            }
            else
            {
                return 0.0;
            }
        }
    }
    else
    {
        qDebug() << "query.exec()-Error: " << this->db.lastError();
        return 0.0;
    }
}

double DB_DataProvider::getAverageNormalPriceOfShopItem( const uint &shopItemID ) const
{
    QSqlQuery query;

    const QString sql = "SELECT avg(NormalPrice) FROM discounter_shopitem WHERE ShopItemID = "
            + QString::number( shopItemID );

    if( query.exec( sql ) )
    {
        if( query.next() )
        {
            bool parseToDoubleSucceeded = false;
            double normalPrice = query.value(0).toDouble( &parseToDoubleSucceeded );

            if( parseToDoubleSucceeded )
            {
                return normalPrice;
            }
            else
            {
                return 0.0;
            }
        }
    }
    else
    {
        qDebug() << "query.exec()-Error: " << this->db.lastError();
        return 0.0;
    }
}

