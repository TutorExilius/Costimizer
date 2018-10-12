#include "discounter.h"

Discounter::Discounter()
: id{ 0 }
, name{ "" }
, location{ "" }
{
}

Discounter::Discounter( const ulong &id, const QString &name, const QString &location )
: id{ id }
, name{ name }
, location{ location }
{
}

const ulong& Discounter::getId() const
{
    return this->id;
}

const QString& Discounter::getName() const
{
    return this->name;
}


const QString& Discounter::getLocation() const
{
    return this->location;
}



/*
bool Discounter::addShopItem( const ShopItem &shopItem )
{
    for( const auto &item : this->shopItems )
    {
        if( item.getId() == shopItem.getId() )
        {
            // Items already exists! Do not push.
            return false;
        }
    }

    this->shopItems.push_back( shopItem );
    return true;
}
*/
