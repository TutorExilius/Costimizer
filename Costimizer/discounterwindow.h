#ifndef DISCOUNTERWINDOW_H
#define DISCOUNTERWINDOW_H

#include <QMainWindow>

#include "discountershopitem.h"

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

    void addDiscounterShopItemsToListWidget( const QMap<ulong,QList<DiscounterShopItem>> &lowPricedDiscounters );

private slots:
    void on_pushButton_toClipboard_clicked();

private:
    QString toHtml( QListWidgetItem *item );

    void insertDiscounterName( const QString &content );
    void insertDiscounterLocation( const QString &content );
    void insertDiscounterShopItem( const QString &content );
    void insertSpacer();
    void insertEmptyLine();

    Ui::DiscounterWindow *ui;
    int rowCount;
    DB_DataProvider *ref_dbDataProvider;
};

#endif // DISCOUNTERWINDOW_H
