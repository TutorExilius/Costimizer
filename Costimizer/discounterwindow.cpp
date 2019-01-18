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
#include <QHeaderView>

#include "db_dataprovider.h"

DiscounterWindow::DiscounterWindow( QWidget *parent, QTableWidget *tableWidget  )
: QMainWindow{ parent }
, ui{ new Ui::DiscounterWindow }
, rowCount{ 0 }
{
    this->ui->setupUi(this);

    this->ui->pushButton_printToPdf->setVisible( false );

    this->ui->verticalLayout_table->addWidget( tableWidget );

    tableWidget->verticalHeader()->setVisible(false);
    tableWidget->horizontalHeader()->setVisible(false);

    tableWidget->setSizeAdjustPolicy( QAbstractScrollArea::AdjustToContents );
    tableWidget->resizeColumnsToContents();
    tableWidget->adjustSize();

    this->adjustSize();

  //  this->updateGeometry();
}

DiscounterWindow::~DiscounterWindow()
{
    delete ui;
}


QString DiscounterWindow::toHtml( QListWidgetItem *item )
{

    return "";
}

void DiscounterWindow::on_pushButton_printToPdf_clicked()
{
    /*
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
    */
}
