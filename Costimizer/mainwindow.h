#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QString>

#include "dataprovider.h"
#include "mylist.h"
#include "discounter.h"
#include "config.h"

// Forward-Declarations
class DiscounterShopItem;

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
    //void loadItemsIntoList();

    void combineEntries();
    QPair<QString,int> splitString( QString item );
    QString buildItemCountedEntryName( const QPair<QString,int> &itemKey );
    void addItem( const QString &item );
    void loadItemsIntoList();
    ShopItem getShopItem( const QString &itemName );

    Ui::MainWindow *ui;
    DataProvider *dataProvider;

    MyList *myShoppingList;

    QList<ShopItem*> myShopItems;
    QList<Discounter*> myDiscounter;
    Config config;
};

#endif // MAINWINDOW_H
