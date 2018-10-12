#include "shopitem.h"

ShopItem::ShopItem( const ulong &id, const QString &name )
: id{ id }
, name{ name }
{
}

const ulong& ShopItem::getId() const
{
    return this->id;
}

const QString& ShopItem::getName() const
{
    return this->name;
}
