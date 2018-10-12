#ifndef DISCOUNTER_H
#define DISCOUNTER_H

#include "shopitem.h"

class Discounter
{
public:
    Discounter();
    Discounter( const ulong &id, const QString &name, const QString &location );

    bool addShopItem( const ShopItem &shopItem );

    const ulong& getId() const
    {
        return this->id;
    }

private:
    ulong id;
    QString name;
    QString location;
};

#endif // DISCOUNTER_H
