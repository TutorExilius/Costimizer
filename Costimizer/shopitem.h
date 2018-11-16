#ifndef SHOPITEM_H
#define SHOPITEM_H

#include <QString>

class ShopItem
{
public:
    ShopItem();
    explicit ShopItem( const ulong &id );

    ShopItem( const ulong &id, const QString &name );

    const ulong& getId() const;
    const QString& getName() const;

private:
    ulong id;
    QString name;
};

#endif // SHOPITEM_H
