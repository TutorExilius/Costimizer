#include "configdialog.h"
#include "ui_configdialog.h"

#include "mainwindow.h"

#include <QMessageBox>

ConfigDialog::ConfigDialog( MainWindow *parentWindow, Config config )
: QDialog{ parentWindow }
, ui{ new Ui::ConfigDialog }
, parentWindow{ parentWindow }
, config{ config }
{
    this->ui->setupUi(this);

        this->setWindowTitle( "Einstellungen" );

    // remove help icon
    Qt::WindowFlags flags = this->windowFlags();
    Qt::WindowFlags helpFlag = Qt::WindowContextHelpButtonHint;
    Qt::WindowFlags closeFlag = Qt::WindowCloseButtonHint;
    this->setWindowFlags( flags & ~helpFlag & ~closeFlag );

    this->adjustSize();

    // read config data ---
    const QString database = config.getValueOf("Database");
    this->ui->textEdit_databaseFilePath->setText( database );
    // ---
}

ConfigDialog::~ConfigDialog()
{
    delete ui;
}

void ConfigDialog::on_pushButton_ok_clicked()
{
    this->config["Database"] = this->ui->textEdit_databaseFilePath->toPlainText();

    this->parentWindow->saveConfig( this->config );

    QMessageBox msg;
    msg.setWindowTitle( "Hinweis: Neustart erforderich" );
    msg.setText( "Die Einstellung wird erst zum nächsten Programmstart wirksam." );
    msg.exec();

    this->close();
}

void ConfigDialog::on_pushButton_cancel_clicked()
{
    this->close();
}
