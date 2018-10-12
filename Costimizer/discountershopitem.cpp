#include "discountershopitem.h"

DiscounterShopItem::DiscounterShopItem( const ulong &discounterId, const ulong &shopItemId,
       const double &normalPrice, const double &offerPrice )
: discounterId{ discounterId }
, shopItemId{ shopItemId }
, normalPrice{ normalPrice }
, offerPrice{ offerPrice }
{
}

ulong DiscounterShopItem::getDiscounterId() const
{
    return this->discounterId;
}

ulong DiscounterShopItem::getShopItemId() const
{
    return this->shopItemId;
}

double DiscounterShopItem::getNormalPrice() const
{
    return this->normalPrice;
}

double DiscounterShopItem::getOfferPrice() const
{
    return this->offerPrice;
}

void DiscounterShopItem::setNormalPrice( const double &normalPrice )
{
    this->normalPrice = normalPrice;
}

void DiscounterShopItem::setOfferPrice( const double &offerPrice )
{
    this->offerPrice = offerPrice;
}
