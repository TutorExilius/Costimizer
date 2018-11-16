#include "configdialog.h"
#include "ui_configdialog.h"

#include "mainwindow.h"

ConfigDialog::ConfigDialog( MainWindow *parentWindow, Config config )
: QDialog{ parentWindow }
, ui{new Ui::ConfigDialog}
, parentWindow{ parentWindow }
, config{ config }
{
    ui->setupUi(this);

    // connections (signal/slot) ---
    QObject::connect( this->ui->pushButton_ok, &QPushButton::clicked,
                      this, &ConfigDialog::onOkClicked,
                      Qt::UniqueConnection );

    QObject::connect( this->ui->radioButton_TXT, &QRadioButton::clicked,
                      this, &ConfigDialog::onRadioButton_TXT_checked,
                      Qt::UniqueConnection );

    QObject::connect( this->ui->radioButton_DB, &QRadioButton::clicked,
                      this, &ConfigDialog::onRadioButton_DB_checked,
                      Qt::UniqueConnection );
    // ---

    // read settings from config ---

    if( config.getValueOf("StorageType") == "TXT" )
    {
        this->ui->radioButton_TXT->setChecked( true );
        emit this->ui->radioButton_TXT->clicked(true);
    }
    else if( config.getValueOf("StorageType") == "DB" )
    {
        this->ui->radioButton_DB->setChecked( true );
        emit this->ui->radioButton_DB->clicked(true);
    }

    this->ui->textEdit_shopItemsFilePath->setText( config.getValueOf("ShopList_TXT") );
    this->ui->textEdit_discounterFilePAth->setText( config.getValueOf("DiscounterList_TXT") );
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

void ConfigDialog::onRadioButton_TXT_checked()
{
 //   this->ui->groupBox_DB->setVisible( false );
    this->ui->groupBox_TXT->setVisible( true );
    this->config["StorageType"] = "TXT";
}

void ConfigDialog::onRadioButton_DB_checked()
{
    this->ui->groupBox_TXT->setVisible( false );
    this->config["StorageType"] = "DB";
   // this->ui->groupBox_DB->setVisible( true );
}

void ConfigDialog::on_textEdit_shopItemsFilePath_textChanged()
{
    this->config["ShopList_TXT"] = this->ui->textEdit_shopItemsFilePath->toPlainText();
}

void ConfigDialog::on_textEdit_discounterFilePAth_textChanged()
{
    this->config["DiscounterList_TXT"] = this->ui->textEdit_discounterFilePAth->toPlainText();
}
