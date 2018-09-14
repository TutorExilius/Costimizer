#ifndef DATASPROVIDER_H
#define DATASPROVIDER_H

#include <QList>
#include <QString>

class DataProvider
{
public:
    DataProvider();
    virtual ~DataProvider();

    virtual QList<QString> getItems() const = 0;
};

#endif // DATASPROVIDER_H
