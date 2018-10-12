#ifndef FILEDATAPROVIDER_H
#define FILEDATAPROVIDER_H

#include "dataprovider.h"
#include "discountershopitem.h"

#include <QList>
#include <QString>

class FileDataProvider : public DataProvider
{
public:
    FileDataProvider();
    virtual ~FileDataProvider();

    virtual QList<ShopItem> getShopItems() const ;
    virtual QList<Discounter> getDiscounter() const;
    virtual QList<DiscounterShopItem> getDiscounterShopItems( const ulong &discounterID ) const;

private:
    void readShopItems( const QString &fullFileName );

    QList<ShopItem> shopItems;
    QList<Discounter> discounter;
    QMap<ulong,QList<DiscounterShopItem>> discounterShopItems;
};

#endif // FILEDATAPROVIDER_H
