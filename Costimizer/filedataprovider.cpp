#include "filedataprovider.h"

#include <fstream>
#include <QDebug>
#include <QMessageBox>
#include <string>
#include <sstream>

#include "shopitem.h"

FileDataProvider::FileDataProvider()
{
    this->readShopItems( R"(C:\Users\exi\Desktop\Costimizer\shopitems.txt)" );
    this->readDiscounters( R"(C:\Users\exi\Desktop\Costimizer\discounter.txt)" );
}

FileDataProvider::~FileDataProvider()
{
}

QList<ShopItem> FileDataProvider::getShopItems() const
{
    return this->shopItems;
}

QList<Discounter> FileDataProvider::getDiscounters() const
{
    return this->discounters;
}

QList<DiscounterShopItem> FileDataProvider::getDiscounterShopItems( const ulong &discounterID ) const
{
    if( this->discounterShopItems.contains(discounterID) )
    {
        return this->discounterShopItems[discounterID];
    }
    else
    {
        return QList<DiscounterShopItem>{};
    }
}

void FileDataProvider::readShopItems( const QString &fullFileName )
{
    std::fstream inFile( fullFileName.toStdString() );

    if( !inFile )
    {
        qDebug() << "File could not be opened";

        QMessageBox *msg = new QMessageBox;
        msg->setWindowTitle( "Error" );
        msg->setText( "File could not be opened" );
        msg->setModal( false );
        msg->exec();

        return;
    }

    std::string line;
    while( std::getline( inFile, line ) )
    {
        std::stringstream ss{ line };

        ulong id = 0;
        std::string name;

        ss >> id;
        ss.get(); // skip/ignore next character (in this case, the tabulator '\t)
        std::getline(ss, name);

        ShopItem shopItem{id, QString::fromStdString(name) };

        this->shopItems.push_back( shopItem );
    }
}

void FileDataProvider::readDiscounters( const QString &fullFileName )
{
    std::fstream inFile( fullFileName.toStdString() );

    if( !inFile )
    {
        qDebug() << "File could not be opened";

        QMessageBox *msg = new QMessageBox;
        msg->setWindowTitle( "Error" );
        msg->setText( "File could not be opened" );
        msg->setModal( false );
        msg->exec();

        return;
    }

    std::string line;
    while( std::getline( inFile, line ) )
    {
        std::stringstream ss{ line };

        ulong id = 0;
        std::string name;
        std::string location;

        ss >> id;
        ss.get(); // skip/ignore next character (in this case, the tabulator '\t)
        std::getline( ss, name, '\t' );
        std::getline( ss, location );

        Discounter discounter{id, QString::fromStdString(name), QString::fromStdString( location ) };

        this->discounters.push_back( discounter );
    }
}
/*

void FileDataProvider::loadShopItems( const QString &fileNamePath ) const
{
    std::fstream inFile( fileNamePath.toStdString() );

    if( !inFile )
    {
        qDebug() << "File could not be opened";

        QMessageBox *msg = new QMessageBox;
        msg->setWindowTitle( "Error" );
        msg->setText( "File could not be opened" );
        msg->setModal( false );
        msg->exec();

        return;
    }

    std::string line;
    while( std::getline( inFile, line ) )
    {
        std::stringstream ss(line);

        ulong id = 0;
        std::string name;

        ss >> id;
        std::getline(ss, name);
        name.erase( 0, 1 );

        ShopItem shopItem{id, QString::fromStdString(name) };

        this->shopItems.

Notiz an mich selbst:    - QVariant überall entfernen
                         - DataProvider hat alle Daten aus den TextDateien
                         - DataProvider stellt diese nach Anfrage zur Verfügung (getter)

    }

    return items;
}

QList<QVariant> FileDataProvider::loadDiscounter( const QString &fileNamePath ) const
{
    QList<QVariant> items;

    return items;
}
*/
