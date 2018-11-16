#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>

#include "config.h"

// Forward-Declarations
class MainWindow;

namespace Ui {
class ConfigDialog;
}

class ConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigDialog( MainWindow *parentWindow, Config config );
    ~ConfigDialog();

private slots:
    void onOkClicked();
    void onRadioButton_TXT_checked();
    void onRadioButton_DB_checked();

    void on_textEdit_shopItemsFilePath_textChanged();

    void on_textEdit_discounterFilePAth_textChanged();

private:
    Ui::ConfigDialog *ui;
    MainWindow *parentWindow;
    Config config;
};

#endif // CONFIGDIALOG_H
