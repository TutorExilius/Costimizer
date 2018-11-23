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

MainWindow::MainWindow( QWidget *parent )
: QMainWindow{ parent }
, ui{ new Ui::MainWindow }
, dbDataProvider{ nullptr }
, config{ R"(C:\Users\exi\Desktop\config.txt)" }
{
    this->ui->setupUi( this );

    this->dbDataProvider = new DB_DataProvider{ R"(C:\Users\exi\Desktop\Costimizer\costimizerDB.sqlite)" };

    this->myShoppingList = new MyList{ this->ui->listView_shoppingList };

    this->ui->listView_shoppingList->setModel( this->myShoppingList );

    QObject::connect( this->ui->action_ber_Qt, &QAction::triggered,
                      this, &MainWindow::onAbout,
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

    qDebug() << item << endl;

    item = item.trimmed();

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

void MainWindow::onAbout()
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
    for( int i=0; i<this->myShoppingList->rowCount(); ++i )
    {
        QModelIndex index = this->myShoppingList->index(i,0);
        auto item = this->myShoppingList->data(index);

        QString itemName = item.value<QString>();
        ShopItem shopItem = this->getShopItem( itemName );

        ulong shopItemId = shopItem.getId();

        QList<DiscounterShopItem> discounterShopItems = this->dbDataProvider->getDiscounterShopItems( shopItemId );

        if( discounterShopItems.size() > 0 )
        {
            qDebug() << "BLABLA";
        }

    }

 /*   std::map<const Discounter*,QList<DiscounterShopItem>> generatedDiscounterShoppingLists;

    for( int i=0; i<this->myShoppingList->rowCount(); ++i )
    {
        QModelIndex index = this->myShoppingList->index(i,0);
        auto item = this->myShoppingList->data(index);

        QString itemName = item.value<QString>();
        ShopItem shopItem = this->getShopItem( itemName );

        ulong shopItemId = shopItem.getId();
        QList<const Discounter*> discounters = this->dataProvider->getDiscountersRefs( shopItemId );

        for( const Discounter *discounter : discounters )
        {
            QList<DiscounterShopItem> discounterItems = this->dataProvider->getDiscounterShopItems( discounter->getId() );

            for( const DiscounterShopItem &discounterShopItem : discounterItems )
            {
                if( discounterShopItem.getShopItemId() == shopItemId )
                {
                    generatedDiscounterShoppingLists[discounter].append(discounterShopItem);
                }
            }
        }
    }

    if( generatedDiscounterShoppingLists.size() )
    {
        for( const auto &key : generatedDiscounterShoppingLists )
        {
            qDebug() << "Key: " << key.first->toString();

            for( const auto &val : key.second )
            {
                qDebug() << val.toString();
            }
        }
    }
    else
    {
        qDebug() << "NOPE, generation nix gutt";
    }
    */
}

ShopListMap MainWindow::reduceToCheapestDiscounterShoppingLists( const ShopListMap &geratedDiscounterShoppingLists ) const
{

    // TODO filtere die günstigsten ShopItems und die Informationen, in welchem Discounter diese zu finden sind

}
