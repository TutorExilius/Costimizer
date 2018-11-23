#include "mainwindow.h"
#include <QApplication>

#include <QDebug>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow *w = nullptr;

    try
    {
        w = new MainWindow;
        w->show();
    }
    catch( const QString &error )
    {
        qDebug() << error;
    }
    catch( ... )
    {
        qDebug() << "Unknown error occurred";
    }

    return a.exec();
}
