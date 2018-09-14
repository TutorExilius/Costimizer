#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>

#include "dataprovider.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void onShiftClicked();
    void onDeleteSelectionsClicked();
    void onReduceOneClicked();

private:
    void loadItemsIntoList();
    void combineDoubleEntries();
    QPair<QString,int> splitString( QString item );
    QString buildItemCountedEntryName( const QPair<QString,int> &itemKey );

    Ui::MainWindow *ui;
    DataProvider *dataProvider;
};

#endif // MAINWINDOW_H
