#ifndef FILEDATAPROVIDER_H
#define FILEDATAPROVIDER_H

#include "dataprovider.h"
#include "discountershopitem.h"

#include <QList>
#include <QString>

class FileDataProvider : public DataProvider
{
public:
    // static ---
    static QString withoutExtension( const QString &fileName );
    // ---

    FileDataProvider( const QString &shopItemDatafile, const QString &discounterDatafile );
    virtual ~FileDataProvider() override;

    virtual QList<ShopItem> getShopItems() const override;
    virtual QList<Discounter> getDiscounters() const override;
    virtual QList<DiscounterShopItem> getDiscounterShopItems( const ulong &discounterID ) const override;

private:
    // static ---
    static QString discounterShopItemsSubdir;
    // ---

    // helper ---
    void readShopItems( const QString &fullFileName );
    void readDiscounters( const QString &fullFileName );
    void readDisounterShopItems( const QString &discounterShopItemsSubdir );
    //---

    QList<ShopItem> shopItems;
    QList<Discounter> discounters;

    // Key: DiscounterID - Value: List of DiscounterShopItems
    QMap<ulong,QList<DiscounterShopItem>> discounterShopItems;
};

#endif // FILEDATAPROVIDER_H
