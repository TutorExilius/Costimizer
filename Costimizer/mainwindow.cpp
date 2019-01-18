#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QList>
#include <QListWidget>
#include <QListWidgetItem>
#include <QStringList>
#include <QMap>
#include <QDebug>
#include <QString>
#include <QPair>
#include <QTableWidget>
#include <QFontDatabase>

#include <map>
#include <string>
#include <sstream>

#include <QItemSelectionModel>
#include <QMessageBox>

#include "db_dataprovider.h"
#include "shopitem.h"
#include "discountershopitem.h"
#include "configdialog.h"
#include "discounterwindow.h"

MainWindow::MainWindow( QWidget *parent )
: QMainWindow{ parent }
, ui{ new Ui::MainWindow }
, dbDataProvider{ nullptr }
, config{ QCoreApplication::applicationDirPath() + "/config.txt" }
{
    this->ui->setupUi( this );

    this->ui->pushButton_reduce1->setVisible( false );

    const QString database = this->config.getValueOf("Database");
    this->dbDataProvider = new DB_DataProvider{ database };

    this->myShoppingList = new MyList{ this->ui->listView_shoppingList };

    this->ui->listView_shoppingList->setModel( this->myShoppingList );

    QObject::connect( this->ui->action_ber_Qt, &QAction::triggered,
                      this, &MainWindow::onAboutQt,
                      Qt::UniqueConnection );

    QObject::connect( this->ui->action_ber_Costimizer, &QAction::triggered,
                      this, &MainWindow::onAboutCostimizer,
                      Qt::UniqueConnection );

    QObject::connect( this->ui->listWidget_items, &QListWidget::itemDoubleClicked,
                      this, &MainWindow::onDoubleClicked,
                      Qt::UniqueConnection );

    QObject::connect( this->ui->pushButton_shiftItem, &QPushButton::clicked,
                      this, &MainWindow::onShiftClicked_simplyfiedVersion,
                      Qt::UniqueConnection );

    /* With Combination Mutltiple items in list  1x,..2x,..3x

    QObject::connect( this->ui->pushButton_shiftItem, &QPushButton::clicked,
                      this, &MainWindow::onShiftClicked,
                      Qt::UniqueConnection );
    */

    QObject::connect( this->ui->pushButton_deleteSelections, &QPushButton::clicked,
                      this, &MainWindow::onDeleteSelectionsClicked,
                      Qt::UniqueConnection );

    QObject::connect( this->ui->pushButton_reduce1, &QPushButton::clicked,
                      this, &MainWindow::onReduceOneClicked,
                      Qt::UniqueConnection );

    QObject::connect( this->ui->action_Settings, &QAction::triggered,
                      this, &MainWindow::onSettingsTriggered,
                      Qt::UniqueConnection );

    this->loadItemsIntoList();
}

MainWindow::~MainWindow()
{
    delete this->myShoppingList;
    delete this->dbDataProvider;
    delete this->ui;
}

void MainWindow::loadItemsIntoList()
{
    auto shopItems = this->dbDataProvider->getShopItems();

    for( const auto &item : shopItems )
    {
        this->ui->listWidget_items->addItem( item.getName() );
    }
}

void MainWindow::saveConfig( const Config config )
{
    this->config = config;
    this->config.writeConfigFile();
}

QString MainWindow::getShopItemName( const ulong &shopItemID ) const
{
    return this->dbDataProvider->getShopItemName( shopItemID );
}

ShopItem MainWindow::getShopItem( const QString &itemName )
{
    return this->dbDataProvider->getShopItem( itemName );
}

/*
void MainWindow::createDiscounterWindows( const QMap<ulong,QList<DiscounterShopItem>> &lowPricedDiscounters,
                                          const QMap<ulong,QList<DiscounterShopItem>> &otherPricedDiscounters,
                                          const QList<ShopItem> &shopItemsWithoutDiscounter )
{
 //   QMap<ulong,DiscounterWindow*> discounterWindows;
    DiscounterWindow *discounterWindows = new DiscounterWindow( this, this->dbDataProvider );
    discounterWindows->addDiscounterShopItemsToListWidget( lowPricedDiscounters, otherPricedDiscounters );
    discounterWindows->addShopItmesToListWidget( shopItemsWithoutDiscounter );
    discounterWindows->show();

    /*
    for( const auto &key : lowPricedDiscounters.keys() )
    {
      //  DiscounterWindow *discounterWindow = new DiscounterWindow( this );
      //  discounterWindow->setWindowTitle( QString::number( key ) );

     //   if( !discounterWindows.contains( key ))

        {
            discounterWindows.insert( key, discounterWindow );
        }
    }

    for( const auto &key : discounterWindows.keys() )
    {
        discounterWindows.value(key)->addDiscounterShopItemToListWidget( lowPricedDiscounters[key] );
        discounterWindows.value(key)->show();
    }
}
*/

QPair<QString,int> MainWindow::splitString( QString item )
{
    QPair<QString,int> pair;

    std::stringstream ss( item.toStdString() );
    bool alreadyCombined = false;

    int quantity = 0;
    char multMarker;

    if( (ss >> quantity >> multMarker) && (quantity != 0 && multMarker == 'x') )
    {
        alreadyCombined = true;
    }

    if( alreadyCombined )
    {
        std::string itemStdStr;
        ss.clear();

        std::getline( ss, itemStdStr );

        item = QString::fromStdString(itemStdStr);
    }

    item = item.trimmed();
    qDebug() << item;

    pair.first = item;
    pair.second = (quantity==0) ? 1 : quantity;

    return pair;
}

QList<ShopItem> MainWindow::getMyShopList() const
{
    QList< ShopItem> shopItems;

    for( int i = 0; i < this->myShoppingList->rowCount(); ++i )
    {
        const QModelIndex index = this->myShoppingList->index(i);
        const QString shopItemName = this->myShoppingList->data( index ).toString() ;

        ShopItem shopItem = this->dbDataProvider->getShopItem( shopItemName );

        shopItems.append( shopItem );
    }

    return shopItems;
}

bool MainWindow::existsInShoppingList( const QString &item )
{
    for( int i = 0; i < this->myShoppingList->rowCount(); ++i )
    {
        QModelIndex index = this->myShoppingList->index(i);
        QString _item = (this->myShoppingList->data( index )).toString();

        if( _item == item )
        {
            return true;
        }
    }

    return false;
}

void MainWindow::combineEntries()
{
    QMap<QString, int> itemsCnt;

    for( int i = 0; i < this->myShoppingList->rowCount(); ++i )
    {
        QModelIndex index = this->myShoppingList->index(i);
        QString item = (this->myShoppingList->data( index )).toString();

        auto pair = this->splitString( item );

        QString itemName = pair.first;
        int quantity = pair.second;

        qDebug() << "\t" << itemName << ":" << quantity;

        itemsCnt[itemName] += quantity;
    }

    this->myShoppingList->clear();

    for( const auto &item : itemsCnt.keys() )
    {
        if( itemsCnt.value(item) <= 0 )
        {
            continue;
        }

        QPair<QString,int> pair{ item, itemsCnt.value(item)};

        QString itemName = this->buildItemCountedEntryName( pair );
        this->myShoppingList->addItem( itemName );
    }
}

QString MainWindow::buildItemCountedEntryName( const QPair<QString,int> &itemKey )
{
    QString itemVal = QString::number(itemKey.second) + "x ";

    if( itemKey.second == 1 )
        itemVal = "";

    return itemVal + itemKey.first;
}

void MainWindow::addItem( const QString &item, bool multipleInsert )
{
    if( item.isEmpty() )
    {
        return;
    }

    if( multipleInsert )
    {
        this->myShoppingList->addItem( item );
    }
    else
    {
        if( !this->existsInShoppingList( item ) )
        {
            this->myShoppingList->addItem( item );
        }
    }
}

void MainWindow::onDoubleClicked( QListWidgetItem *item )
{
    this->addItem( "1x " + this->splitString(item->text()).first );

    this->combineEntries();

    this->ui->listWidget_items->reset();
}

void MainWindow::onShiftClicked_simplyfiedVersion()
{
    QList<QListWidgetItem*> selectedItems = this->ui->listWidget_items->selectedItems();

    for( const auto &item : selectedItems )
    {
        this->addItem( this->splitString(item->text()).first, false );
    }
}

void MainWindow::onShiftClicked()
{
    QList<QListWidgetItem*> selectedItems = this->ui->listWidget_items->selectedItems();

    for( const auto &item : selectedItems )
    {
        this->addItem( "1x " + this->splitString(item->text()).first );
    }

    this->combineEntries();
}

void MainWindow::onDeleteSelectionsClicked()
{
    while( this->ui->listView_shoppingList->selectionModel()->selectedIndexes().size() > 0 )
    {
        // delete first selection
        auto firstSeletctionIndex = this->ui->listView_shoppingList->selectionModel()->selectedIndexes().at(0);
        this->myShoppingList->removeRow( firstSeletctionIndex.row() );
    }
}

void MainWindow::onReduceOneClicked()
{
    bool hasSelections = false;

    for( const auto &selectedItem : this->ui->listView_shoppingList->selectionModel()->selectedIndexes() )
    {
        hasSelections = true;
        this->myShoppingList->addItem( "-1x " + this->splitString(selectedItem.data().toString()).first );
    }

    if( hasSelections )
    {
        this->combineEntries();
    }
}

void MainWindow::onAboutCostimizer()
{
    QMessageBox::about( this, "About Costimizer", "A \"C++ Let's Try [Qt]\" - Community Project\nof Tutor Exilius (http://twitch.tv/TutorExilius)");
}

void MainWindow::onAboutQt()
{
    QMessageBox::aboutQt( this );
}

void MainWindow::onSettingsTriggered()
{
    qDebug() << "Settings...";

    ConfigDialog *settings = new ConfigDialog{ this, this->config };
    settings->exec();
}

void MainWindow::on_pushButton_generateLists_clicked()
{
    // Discounter -> List(ShopItemName->Price (real/extimated))
  //  QMap<QString,QList<QPair<QString,Price>>> items;
    std::map<QString,QList<QPair<QString,Price>>> items;

    QList<ShopItem> shopItems = this->getMyShopList();

    if( shopItems.isEmpty() )
    {
        return;
    }

    for( const auto &discounter : this->dbDataProvider->getDiscounters() )
    {
        items.insert( std::make_pair(discounter.getName(), QList<QPair<QString,Price>>{}) );

        for( const auto &shopItem : shopItems )
        {
            double price = this->dbDataProvider->getNormalPrice(
                        shopItem.getId(), discounter.getId() );

            PriceType type;

            if( price > 0.0 )
            {
                type = PriceType::REAL;
            }
            else
            {
                price = this->dbDataProvider->getAverageNormalPriceOfShopItem( shopItem.getId() );
                type = PriceType::ESTIMATED;
            }

            items[discounter.getName()].append( QPair<QString,Price>{shopItem.getName(), Price{ price, type }} );
        }
    }

    // TODO: calculate all estimated prices

    // Discounter -> sum of all ShopItems-normalPrices
    std::map<QString,double> sumAmountDiscounter;
    std::map<QString,double> coveragePriceDiscounter;

    for( const auto &mapItem : items )
    {
        double sumPrice = 0.0;
        int cntRealPrices = 0;
        const int cntPrices = shopItems.size();

        for( const auto &pair : items[mapItem.first] )
        {

            sumPrice += pair.second.price;

            if( pair.second.priceType == PriceType::REAL )
            {
                ++cntRealPrices;
            }
        }

        sumAmountDiscounter.insert( std::make_pair(mapItem.first, sumPrice) );
        coveragePriceDiscounter.insert( std::make_pair(mapItem.first,
                    cntRealPrices * 100 / cntPrices ) );
    }

    int maxLength = 0;

    // find max string shopITem Name
    for( const auto &shopItem : shopItems )
    {
        const QString shopItemName = shopItem.getName();

        if( maxLength < shopItemName.size() )
        {
            maxLength = shopItemName.size();
        }
    }

    // TODO output data

    QFont font("Monospace");
    font.setStyleHint(QFont::TypeWriter);

    QTableWidget *tableWidget = new QTableWidget( this );
    tableWidget->setFont( font );
    tableWidget->setColumnCount( items.size() + 1 );
    tableWidget->setRowCount( shopItems.size() + 2 );

    int colIndex = 0;
    int rowIndex = 0;

    QTableWidgetItem *item = new QTableWidgetItem( "" );
    tableWidget->setItem( 0, colIndex++, item );

    // Fill Disounternames
    for( const auto &keyVal : items )
    {
        item = new QTableWidgetItem( keyVal.first );
        item->setTextColor( QColor::fromRgb(66,99,185) );

        font = item->font();
        font.setPointSize( 12 );
        font.setBold( true );

        item->setFont( font ) ;
        tableWidget->setItem( 0, colIndex++, item);
    }

    ++rowIndex;

    // Fill ShopitemNames + Prices/EstimatedPrices
    for( const auto &shopItem : shopItems )
    {
        colIndex = 0;

        item = new QTableWidgetItem( shopItem.getName() );
        tableWidget->setItem( rowIndex, 0, item );

        for( const auto &keyVal : items )
        {

            for( const auto &val : keyVal.second )
            {
                if( val.first == shopItem.getName() )
                {
                    QString out;
                    bool hoverColor = false;
                    if( val.second.priceType == PriceType::ESTIMATED )
                    {
                        out = "Ø = ";
                        hoverColor = true;
                    }

                   out +=  QString::number(val.second.price, 'f', 2 );

                   item = new QTableWidgetItem( out );

                   if( hoverColor )
                   {
                        item->setTextColor( QColor::fromRgb(245,100,0) );
                   }

                   tableWidget->setItem( rowIndex, ++colIndex, item );

                }
            }
        }

        ++rowIndex;
    }

    // Fill Gesamtsuppe and sumAmounts
    item = new QTableWidgetItem( "GESAMTSUMME");

    font = item->font();
    font.setPointSize( 10 );
    font.setBold( true );

    item->setFont( font );

    tableWidget->setItem( rowIndex, 0, item );

    colIndex = 0;


    for( const auto &keyVal : items )
    {
        item = new QTableWidgetItem( QString::number(sumAmountDiscounter.at(keyVal.first), 'f', 2 ) );

        font = item->font();
        font.setPointSize( 10 );
        font.setBold( true );

        item->setFont( font );

        tableWidget->setItem( rowIndex, ++colIndex, item );
    }

    //tableWidget->show();


    DiscounterWindow *resultTable = new DiscounterWindow{ this, tableWidget };
    resultTable->show();

    /*
    QMap<Discounter, QList<QPair<ShopItem,
    QList<DiscounterShopItem> allLowPricedDiscounters;
    QList<DiscounterShopItem> allPricedDiscounters;
    QList<ShopItem> shopItemsWithoutDiscounter;

    for( int i=0; i<this->myShoppingList->rowCount(); ++i )
    {
        QModelIndex index = this->myShoppingList->index(i,0);
        auto item = this->myShoppingList->data(index);

        QString itemName = item.value<QString>();
        QPair<QString, int> itemShopPair = MainWindow::splitString( itemName );
        ShopItem shopItem = this->getShopItem( itemShopPair.first );

        ulong shopItemId = shopItem.getId();

        QList<DiscounterShopItem> tmp_lowPricedDiscounters = this->dbDataProvider->getLowPricedDiscounters( shopItemId );
        QList<DiscounterShopItem> tmp_allPricedDiscounters = this->dbDataProvider->getAllPricedDiscounters( shopItemId );

        QList<uint> alreadyCollectedDiscounters;

        // Collect all DiscounterShopItems
        for( const auto &lowPricedDiscounter : tmp_lowPricedDiscounters )
        {
            allLowPricedDiscounters.push_back( lowPricedDiscounter );
            alreadyCollectedDiscounters.push_back( lowPricedDiscounter.getDiscounterId() );
        }

        // Collect all DiscounterShopItems
        for( const auto &pricedDiscounter : tmp_allPricedDiscounters )
        {
            if( !alreadyCollectedDiscounters.contains( pricedDiscounter.getDiscounterId() ) )
            {
                allPricedDiscounters.push_back( pricedDiscounter );
            }
        }

        if( tmp_allPricedDiscounters.size() == 0 )
        {
           shopItemsWithoutDiscounter.push_back( shopItem );
        }
    }

    // Build Map
    QMap<ulong,QList<DiscounterShopItem>> mapLowPricedDiscounters;
    QMap<ulong,QList<DiscounterShopItem>> mapOtherPricedDiscounters;

    for( const auto &lowPricedDiscounter : allLowPricedDiscounters )
    {
        const ulong discounterId = lowPricedDiscounter.getDiscounterId();

        if( !mapLowPricedDiscounters.contains( discounterId ) )
        {
            mapLowPricedDiscounters.insert( discounterId, QList<DiscounterShopItem>{} );
        }

        mapLowPricedDiscounters[discounterId].append( lowPricedDiscounter );
    }

    for( const auto &pricedDiscounter : allPricedDiscounters )
    {
        const ulong discounterId = pricedDiscounter.getDiscounterId();

        if( !mapOtherPricedDiscounters.contains( discounterId ) )
        {
            mapOtherPricedDiscounters.insert( discounterId, QList<DiscounterShopItem>{} );
        }

        mapOtherPricedDiscounters[discounterId].append( pricedDiscounter );
    }

    this->createDiscounterWindows( mapLowPricedDiscounters, mapOtherPricedDiscounters, shopItemsWithoutDiscounter );
*/

}
