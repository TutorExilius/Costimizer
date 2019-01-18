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

// type-definitions ---
enum class PriceType { UNKNOWN = 0, REAL, ESTIMATED };

struct Price
{
    Price( const double &price = 0.0, const PriceType type = PriceType::UNKNOWN )
        : price{ price }, priceType{ type }
    {
    }

    double price;
    PriceType priceType;
};
// ---

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static QPair<QString,int> splitString( QString item );

    explicit MainWindow( QWidget *parent = nullptr );
    virtual ~MainWindow();

    void saveConfig( const Config config );

    QString getShopItemName( const ulong &id ) const;

public slots:
    void onDoubleClicked( QListWidgetItem *item );
    void onShiftClicked();
    void onShiftClicked_simplyfiedVersion();
    void onDeleteSelectionsClicked();
    void onReduceOneClicked();

private slots:
    void onAboutQt();
    void onAboutCostimizer();
    void onSettingsTriggered();
    void on_pushButton_generateLists_clicked();

private:
    void combineEntries();
    QString buildItemCountedEntryName( const QPair<QString,int> &itemKey );
    void addItem( const QString &item, bool multipleInsert = true );
    void loadItemsIntoList();
    ShopItem getShopItem( const QString &itemName );
    bool existsInShoppingList( const QString &item );
    //void createDiscounterWindows( const QMap<ulong,QList<DiscounterShopItem>> &lowPricedDiscounters,
      //                            const QMap<ulong,QList<DiscounterShopItem>> &otherPricedDiscounters,
        //                          const QList<ShopItem> &shopItemsWithoutDiscounter );
    QList<ShopItem> getMyShopList() const;

    Ui::MainWindow *ui;

    DB_DataProvider *dbDataProvider;
    MyList *myShoppingList;    
    Config config;
};

#endif // MAINWINDOW_H
