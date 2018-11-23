#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QString>

#include <map>

#include "db_dataprovider.h"

#include "mylist.h"
#include "config.h"

// Forward-Declarations
class DiscounterShopItem;

// Definitions
using ShopListMap = std::map<const Discounter*,QList<DiscounterShopItem>>;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow( QWidget *parent = nullptr );
    virtual ~MainWindow();

    void saveConfig( const Config config );

    QString getShopItemName( const ulong &id ) const;

public slots:
    void onDoubleClicked( QListWidgetItem *item );
    void onShiftClicked();
    void onDeleteSelectionsClicked();
    void onReduceOneClicked();

private slots:
    void onAbout();
    void onSettingsTriggered();
    void on_pushButton_generateLists_clicked();

private:
    void combineEntries();
    QPair<QString,int> splitString( QString item );
    QString buildItemCountedEntryName( const QPair<QString,int> &itemKey );
    void addItem( const QString &item );
    void loadItemsIntoList();
    ShopItem getShopItem( const QString &itemName );
    ShopListMap reduceToCheapestDiscounterShoppingLists( const ShopListMap &geratedDiscounterShoppingLists ) const;

    Ui::MainWindow *ui;

    DB_DataProvider *dbDataProvider;
    MyList *myShoppingList;    
    Config config;
};

#endif // MAINWINDOW_H
