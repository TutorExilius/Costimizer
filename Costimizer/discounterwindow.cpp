#include "discounterwindow.h"
#include "ui_discounterwindow.h"

//#include <QClipboard>
//#include <QMimeData>
#include <QDebug>
#include <QTextBrowser>
#include <QTextEdit>
#include <QListWidgetItem>
#include <QColor>
#include <QPrinter>
#include <QTextCursor>
#include <QDir>

#include "db_dataprovider.h"

DiscounterWindow::DiscounterWindow( QWidget *parent, DB_DataProvider *ref_dbDataProvider )
: QMainWindow{ parent }
, ui{ new Ui::DiscounterWindow }
, rowCount{ 0 }
, ref_dbDataProvider{ ref_dbDataProvider }
{
    this->ui->setupUi(this);
}

DiscounterWindow::~DiscounterWindow()
{
    delete ui;
}

void DiscounterWindow::addDiscounterShopItemsToListWidget( const QMap<ulong,QList<DiscounterShopItem>> &lowPricedDiscounters,
                                                           const QMap<ulong,QList<DiscounterShopItem>> &otherPricedDiscounters )
{
    for( const auto &key : lowPricedDiscounters.keys() )
    {
        double sumLowPriced = 0.0;
        double sumPrice = 0.0;

        const Discounter discounter =  this->ref_dbDataProvider->getDiscounter( key );

        const QString discounterName = discounter.getName();
        const QString discounterLocation = discounter.getLocation();

        this->insertDiscounterName( discounterName );
        this->insertDiscounterLocation( discounterLocation );

        this->insertEmptyLine();

        for( const auto &value : lowPricedDiscounters.value( key ) )
        {
            QString shopItemName = this->ref_dbDataProvider->getShopItemName( value.getShopItemId() );

            QString text =  "(N:" + QString::number( value.getNormalPrice() );
            text += ( value.getOfferPrice() > 0.0 )
                    ? "/A:" + QString::number( value.getOfferPrice() ) + ") " + shopItemName
                    : ") " + shopItemName;

            this->insertDiscounterShopItem( text );

            sumLowPriced += value.getNormalPrice();
        }

        sumPrice = sumLowPriced;

        for( const auto &value : otherPricedDiscounters.value( key ) )
        {
            const QString shopItemName = this->ref_dbDataProvider->getShopItemName( value.getShopItemId() );

            QString text =  "(N:" + QString::number( value.getNormalPrice() );
            text += ( value.getOfferPrice() > 0.0 )
                    ? "/A:" + QString::number( value.getOfferPrice() ) + ") " + shopItemName
                    :  ") " + shopItemName;


            this->insertDiscounterShopItem( text, Qt::gray );

            sumPrice += value.getNormalPrice() ;
        }

        this->insertEmptyLine();

        this->insertDiscounterShopItem( QString::number(sumLowPriced) );
        this->insertDiscounterShopItem( QString::number(sumPrice), Qt::gray );

        this->insertEmptyLine();
        this->insertSpacer();
        this->insertEmptyLine();
    }
}

void DiscounterWindow::addShopItmesToListWidget( const QList<ShopItem> &shopItemsWithoutDiscounter )
{
    this->insertEmptyLine();

    for( const auto &shopItem : shopItemsWithoutDiscounter )
    {
        this->insertDiscounterShopItem( shopItem.getName(), QColor::fromRgb( 255, 165, 0 ) );
    }
}

void DiscounterWindow::insertDiscounterName( const QString &content )
{
    QListWidgetItem *item = new QListWidgetItem;
    item->setText( content );
    item->setTextColor( Qt::red );
    item->setFont( QFont( "Verdana", 11, 75	) );

    this->ui->listWidget_discounterShopItems->insertItem( this->rowCount, item );
    ++this->rowCount;
}

void DiscounterWindow::insertDiscounterLocation( const QString &content )
{
    QListWidgetItem *item = new QListWidgetItem;
    item->setText( content );
    item->setTextColor( Qt::blue );
    item->setFont( QFont( "Verdana", 9, 50, true ) );

    this->ui->listWidget_discounterShopItems->insertItem( this->rowCount, item );
    ++this->rowCount;
}

void DiscounterWindow::insertDiscounterShopItem( const QString &content, const QColor &color )
{
    QListWidgetItem *item = new QListWidgetItem;
    item->setText( content );
    item->setTextColor( color );
    item->setFont( QFont( "Verdana", 10 ) );

    this->ui->listWidget_discounterShopItems->insertItem( this->rowCount, item );
    ++this->rowCount;
}

void DiscounterWindow::insertSpacer()
{
    QListWidgetItem *item = new QListWidgetItem;
    item->setText( "----------------" );
    item->setFont( QFont( "Verdana", 6 ) );

    this->ui->listWidget_discounterShopItems->addItem( item );
    ++this->rowCount;
}

void DiscounterWindow::insertEmptyLine()
{
    QListWidgetItem *item = new QListWidgetItem;
    item->setText( "" );
    item->setFont( QFont( "Verdana", 6 ) );

    this->ui->listWidget_discounterShopItems->addItem( item );
    ++this->rowCount;
}

QString DiscounterWindow::toHtml( QListWidgetItem *item )
{
    QTextEdit textEdit;

    textEdit.setText( item->text() );

    textEdit.setFont( item->font() );

    QString html = textEdit.toHtml();

    return html.replace( "<body style=\"", "<body style=\"color: " + item->textColor().name() + "; ");
}

void DiscounterWindow::on_pushButton_printToPdf_clicked()
{
    QTextEdit *textEdit = new QTextEdit;

    for( int i=0; i<this->rowCount; ++i )
    {
        textEdit->setHtml( textEdit->toHtml() + this->toHtml( this->ui->listWidget_discounterShopItems->item(i) ) );
    }

    QTextDocument document;
    document.setHtml( textEdit->toHtml() );

    QPrinter printer(QPrinter::PrinterResolution);

    printer.setFontEmbeddingEnabled( true );
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QPrinter::A4);
    printer.setOutputFileName(QDir::homePath() + "/costimizer.pdf" );
    printer.setPageMargins(QMarginsF(15, 15, 15, 15));

    document.print(&printer);
}
