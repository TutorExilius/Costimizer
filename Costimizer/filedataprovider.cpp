#include "filedataprovider.h"

#include <fstream>
#include <QDebug>
#include <QDir>
#include <QList>
#include <QMessageBox>
#include <string>
#include <sstream>

#include "shopitem.h"
#include "discountershopitem.h"

// static ---
QString FileDataProvider::discounterShopItemsSubdir = R"(C:\Users\exi\Desktop\Costimizer\discounterpreise)";
// ---

FileDataProvider::FileDataProvider( const QString &shopItemDatafile, const QString &discounterDatafile )
{
    this->readShopItems( shopItemDatafile );
    this->readDiscounters( discounterDatafile );
    this->readDisounterShopItems( FileDataProvider::discounterShopItemsSubdir );
}

FileDataProvider::~FileDataProvider()
{
}

QList<ShopItem> FileDataProvider::getShopItems() const
{
    return this->shopItems;
}

Discounter* FileDataProvider::getDiscounter( const ulong &discounterId )
{
    for( Discounter &discounter : this->discounters )
    {
        if( discounter.getId() == discounterId )
        {
            return &discounter;
        }
    }

    return nullptr;
}

const Discounter* FileDataProvider::getDiscounter( const ulong &discounterId ) const
{
    for( const Discounter &discounter : this->discounters )
    {
        if( discounter.getId() == discounterId )
        {
            return &discounter;
        }
    }

    return nullptr;
}

QList<Discounter> FileDataProvider::getDiscounters() const
{
    return this->discounters;
}

QList<const Discounter*> FileDataProvider::getDiscountersRefs( const ulong &shopItemId ) const
{
    QList<const Discounter*> discounterList;

    for( ulong discounterId : this->discounterShopItems.keys() )
    {
        QList<DiscounterShopItem> discounterShopItems = this->discounterShopItems.value( discounterId );

        for( const auto &discounterShopItem : discounterShopItems )
        {
            if( discounterShopItem.getShopItemId() == shopItemId )
            {
                const Discounter *discounter = this->getDiscounter( discounterId );

                if( discounter != nullptr )
                {
                    discounterList.push_back( discounter );
                }
            }
        }
    }

    return discounterList;
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

void FileDataProvider::readDisounterShopItems( const QString &discounterShopItemsSubdir )
{
    QDir export_folder( discounterShopItemsSubdir );

    export_folder.setNameFilters( QStringList("*.txt") );

    QStringList fileList = export_folder.entryList();

    foreach( QString fileName, fileList)
    {
        qDebug() << fileName;
        QString cleanFileName = FileDataProvider::withoutExtension( fileName );

        QFile file( discounterShopItemsSubdir + "\\" + fileName );
        if( !file.open( QFile::ReadOnly|QFile::Text ) )
        {
            qDebug() << " Could not open the file '"
                     << fileName << "' for reading";
            continue;
        }

        QList<DiscounterShopItem> list;
        unsigned long disounterId = cleanFileName.toULong();

        while( !file.atEnd() )
        {
            ulong shopItemId;
            double normalPrice;
            double offerPrice;

            QString line = file.readLine();

            if( !line.isEmpty() )
            {
                QTextStream in( &line );
                in >> shopItemId >> normalPrice >> offerPrice;

                DiscounterShopItem item{ disounterId, shopItemId, normalPrice, offerPrice };

                list.append( item );
            }
        }

        this->discounterShopItems.insert( disounterId, list );
    }
}

QString FileDataProvider::withoutExtension( const QString &fileName)
{
    return fileName.left( fileName.lastIndexOf(".") );
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
