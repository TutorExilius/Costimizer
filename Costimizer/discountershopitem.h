#ifndef DISCOUNTERSHOPITEM_H
#define DISCOUNTERSHOPITEM_H

#include <QtGlobal>

class DiscounterShopItem
{
public:
    DiscounterShopItem() = default;
    explicit DiscounterShopItem( const ulong &discounterId, const ulong &shopItemId,
                                 const double &normalPrice, const double &offerPrice );

    ulong getDiscounterId() const;
    ulong getShopItemId() const;
    double getNormalPrice() const;
    double getOfferPrice() const;

    void setNormalPrice( const double &normalPrice );
    void setOfferPrice( const double &offerPrice );

    QString toString() const;

private:
    ulong discounterId;
    ulong shopItemId;
    double normalPrice;
    double offerPrice;
};

#endif // DISCOUNTERSHOPITEM_H
