#ifndef DATASPROVIDER_H
#define DATASPROVIDER_H

#include <QList>
#include <QString>
#include <QMap>

#include "shopitem.h"
#include "discountershopitem.h"
#include "discounter.h"

class DataProvider
{
public:
    DataProvider();
    virtual ~DataProvider();

    virtual QList<ShopItem> getShopItems() const = 0;
    virtual Discounter* getDiscounter( const ulong &discounterId ) = 0;
    virtual const Discounter* getDiscounter( const ulong &discounterId ) const = 0;
    virtual QList<Discounter> getDiscounters() const = 0;
    virtual QList<const Discounter*> getDiscountersRefs( const ulong &shopItemId ) const = 0;
    virtual QList<DiscounterShopItem> getDiscounterShopItems( const ulong &discounterID ) const = 0;

private:
    DataProvider( const DataProvider &obj ) = delete;
    DataProvider( DataProvider &&obj ) = delete;
    DataProvider& operator=( const DataProvider &obj ) = delete;
    DataProvider& operator=( const DataProvider &&obj ) = delete;
};

#endif // DATASPROVIDER_H
