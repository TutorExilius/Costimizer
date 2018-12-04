#ifndef DISCOUNTERWINDOW_H
#define DISCOUNTERWINDOW_H

#include <QMainWindow>

#include "discountershopitem.h"
#include "shopitem.h"

// Forward-Declarations
class DB_DataProvider;
class QListWidgetItem;

namespace Ui {
    class DiscounterWindow;
}

class DiscounterWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DiscounterWindow( QWidget *parent, DB_DataProvider *ref_dbDataProvider );
    ~DiscounterWindow();

    void addDiscounterShopItemsToListWidget( const QMap<ulong,QList<DiscounterShopItem>> &lowPricedDiscounters,
                                             const QMap<ulong,QList<DiscounterShopItem>> &otherPricedDiscounters );
    void addShopItmesToListWidget( const QList<ShopItem> &shopItemsWithoutDiscounter );

private slots:
    void on_pushButton_printToPdf_clicked();

private:
    QString toHtml( QListWidgetItem *item );

    void insertDiscounterName( const QString &content );
    void insertDiscounterLocation( const QString &content );
    void insertDiscounterShopItem( const QString &content, const QColor &color = Qt::black );
    void insertSpacer();
    void insertEmptyLine();

    Ui::DiscounterWindow *ui;
    int rowCount;
    DB_DataProvider *ref_dbDataProvider;
};

#endif // DISCOUNTERWINDOW_H
