#include "filedataprovider.h"

#include <fstream>
#include <QDebug>
#include <QMessageBox>
#include <string>
#include <sstream>

FileDataProvider::FileDataProvider()
{

}

FileDataProvider::~FileDataProvider()
{

}

QList<QString> FileDataProvider::getItems() const
{
    QList<QString> items;

    std::fstream inFile( YOUT_FILE_PATH_HERE );

    if( !inFile )
    {
        qDebug() << "File could not be opened";

        QMessageBox *msg = new QMessageBox;
        msg->setWindowTitle( "Error" );
        msg->setText( "File could not be opened" );
        msg->setModal( false );
        msg->exec();


        return items;
    }

    std::string line;
    while( std::getline( inFile, line ) )
    {
        std::stringstream ss(line);

        std::string id;
        std::string item;

        ss >> id;
        std::getline(ss, item);
        item.erase( 0, 1 );

        items.append( QString::fromStdString(item) );
    }

    return items;
}
