#include "configdialog.h"
#include "ui_configdialog.h"

#include "mainwindow.h"

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

    // connections (signal/slot) ---
    QObject::connect( this->ui->pushButton_ok, &QPushButton::clicked,
                      this, &ConfigDialog::onOkClicked,
                      Qt::UniqueConnection );

    // TODO:

    // read config data ---
    QString database = config.getValueOf("Database");
    // ---
}

ConfigDialog::~ConfigDialog()
{
    delete ui;
}

void ConfigDialog::onOkClicked()
{
    this->parentWindow->saveConfig( this->config );
    this->close();
}

void ConfigDialog::on_pushButton_cancel_clicked()
{
    this->close();
}
