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
, config{ R"(C:\Users\exi\Desktop\config.txt)" }
{
    this->ui->setupUi( this );

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
                      this, &MainWindow::onShiftClicked,
                      Qt::UniqueConnection );

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
    }*/
}

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
    qDebug() << item << endl;

    pair.first = item;
    pair.second = (quantity==0) ? 1 : quantity;

    return pair;
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

void MainWindow::addItem( const QString &item )
{
    if( item.size() > 0 )
    {
        this->myShoppingList->addItem( item );
    }
}

void MainWindow::onDoubleClicked( QListWidgetItem *item )
{
    this->addItem( "1x " + this->splitString(item->text()).first );

    this->combineEntries();

    this->ui->listWidget_items->reset();
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
}
