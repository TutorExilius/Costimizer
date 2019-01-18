#ifndef DISCOUNTERWINDOW_H
#define DISCOUNTERWINDOW_H

#include <QMainWindow>
#include <QTableWidget>

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
    explicit DiscounterWindow( QWidget *parent, QTableWidget *tableWidget );
    ~DiscounterWindow();

private slots:
    void on_pushButton_printToPdf_clicked();

private:
    QString toHtml( QListWidgetItem *item );

    Ui::DiscounterWindow *ui;
    int rowCount;
};

#endif // DISCOUNTERWINDOW_H
