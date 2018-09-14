#ifndef FILEDATAPROVIDER_H
#define FILEDATAPROVIDER_H

#include "dataprovider.h"

#include <QList>
#include <QString>

class FileDataProvider : public DataProvider
{
public:
    FileDataProvider();
    virtual ~FileDataProvider();

    virtual QList<QString> getItems() const;
};

#endif // FILEDATAPROVIDER_H
