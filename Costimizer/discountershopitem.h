#ifndef DISCOUNTERSHOPITEM_H
#define DISCOUNTERSHOPITEM_H

#include <QtGlobal>

class DiscounterShopItem
{
public:
    explicit DiscounterShopItem( const ulong &discounterId, const ulong &shopItemId,
                                 const double &normalPrice, const double &offerPrice );

    ulong getDiscounterId() const;
    ulong getShopItemId() const;
    double getNormalPrice() const;
    double getOfferPrice() const;

    void setNormalPrice( const double &normalPrice );
    void setOfferPrice( const double &offerPrice );

private:
    ulong discounterId;
    ulong shopItemId;
    double normalPrice;
    double offerPrice;
};

#endif // DISCOUNTERSHOPITEM_H
