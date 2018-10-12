#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QString>

#include "dataprovider.h"
#include "mylist.h"
#include "discounter.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();

    QString getShopItemName( const ulong &id ) const;

public slots:
    void onDoubleClicked( QListWidgetItem *item );
    void onShiftClicked();
    void onDeleteSelectionsClicked();
    void onReduceOneClicked();
    void onAbout();

private:
    //void loadItemsIntoList();

    void combineEntries();
    QPair<QString,int> splitString( QString item );
    QString buildItemCountedEntryName( const QPair<QString,int> &itemKey );
    void addItem( const QString &item );
    void loadItemsIntoList();

    Ui::MainWindow *ui;
    DataProvider *dataProvider;

    MyList *myShoppingList;

    QList<ShopItem*> myShopItems;
    QList<Discounter*> myDiscounter;
};

#endif // MAINWINDOW_H
