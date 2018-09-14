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

#include <string>
#include <sstream>

#include "filedataprovider.h"

MainWindow::MainWindow( QWidget *parent )
: QMainWindow{ parent }
, ui{ new Ui::MainWindow }
, dataProvider{ new FileDataProvider }
{
    this->ui->setupUi(this);

    QObject::connect( this->ui->pushButton_shiftItem, &QPushButton::clicked,
                      this, &MainWindow::onShiftClicked,
                      Qt::UniqueConnection );

    QObject::connect( this->ui->pushButton_deleteSelections, &QPushButton::clicked,
                      this, &MainWindow::onDeleteSelectionsClicked,
                      Qt::UniqueConnection );

    QObject::connect( this->ui->pushButton_reduce1, &QPushButton::clicked,
                      this, &MainWindow::onReduceOneClicked,
                      Qt::UniqueConnection );

    this->loadItemsIntoList();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadItemsIntoList()
{
    auto list = this->dataProvider->getItems();

    for( const auto &item : list )
    {
        this->ui->listWidget_items->addItem( item );
    }
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

void MainWindow::combineDoubleEntries()
{
    QMap<QString, int> itemsCnt;

    for( int i = 0; i < this->ui->listWidget_shoppingList->count(); i++ )
    {
        QString item = this->ui->listWidget_shoppingList->item(i)->text();

        auto pair = this->splitString( item );

        QString itemName = pair.first;
        int quantity = pair.second;

        itemsCnt[itemName]++;

        if( quantity > 1 )
            itemsCnt[itemName] += quantity-1;
    }

    this->ui->listWidget_shoppingList->clear();

    for( const auto &item : itemsCnt.keys() )
    {
        QPair<QString,int> pair{ item, itemsCnt.value(item)};

        QString itemName = this->buildItemCountedEntryName( pair );
        this->ui->listWidget_shoppingList->addItem( itemName );
    }
}

QString MainWindow::buildItemCountedEntryName( const QPair<QString,int> &itemKey )
{
    QString itemVal = QString::number(itemKey.second) + "x ";

    if( itemKey.second == 1 )
        itemVal = "";

    return itemVal + itemKey.first;
}

void MainWindow::onShiftClicked()
{
    QList<QListWidgetItem*> selectedItems = this->ui->listWidget_items->selectedItems();

    for( const auto &item : selectedItems )
    {
        this->ui->listWidget_shoppingList->addItem( item->clone() );
    }

    this->combineDoubleEntries();
}

void MainWindow::onDeleteSelectionsClicked()
{
    QList<QListWidgetItem*> selectedItems = this->ui->listWidget_shoppingList->selectedItems();

    for( const auto &item : selectedItems )
    {
        delete item; // removes also item from list

        qDebug() << this->ui->listWidget_shoppingList->count();
    }
}

void MainWindow::onReduceOneClicked()
{
    QMap<QString, int> itemsCnt;

    QList<QListWidgetItem*> selectedItems = this->ui->listWidget_shoppingList->selectedItems();
    QList<QListWidgetItem*> keepItems;

    for( int i = 0; i < this->ui->listWidget_shoppingList->count(); i++ )
    {
        QListWidgetItem *widgetItem = this->ui->listWidget_shoppingList->item(i);
        auto pair = this->splitString( widgetItem->text() );

        if( selectedItems.contains(widgetItem) )
        {
            if( pair.second == 1 )
            {
                continue;
            }
            else
            {
                pair.second--;
            }
        }

        itemsCnt.insert( pair.first, pair.second );
    }

    this->ui->listWidget_shoppingList->clear();

    for( auto const &item : itemsCnt.keys() )
    {
        QPair<QString,int> pair{item,itemsCnt.value(item)};
        this->ui->listWidget_shoppingList->addItem( this->buildItemCountedEntryName(pair));
    }
}
