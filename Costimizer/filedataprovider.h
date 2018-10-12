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

    virtual QList<ShopItem> getShopItems() const override;
    virtual QList<Discounter> getDiscounters() const override;
    virtual QList<DiscounterShopItem> getDiscounterShopItems( const ulong &discounterID ) const override;

private:
    void readShopItems( const QString &fullFileName );
    void readDiscounters( const QString &fullFileName );

    QList<ShopItem> shopItems;
    QList<Discounter> discounters;
    QMap<ulong,QList<DiscounterShopItem>> discounterShopItems;
};

#endif // FILEDATAPROVIDER_H
