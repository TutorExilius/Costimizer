#ifndef SHOPITEM_H
#define SHOPITEM_H

#include <QString>

class ShopItem
{
public:
    ShopItem();
    explicit ShopItem( const uint &id );
    explicit ShopItem( const uint &id, const QString &name );

    const uint& getId() const;
    const QString& getName() const;

private:
    uint id;
    QString name;
};

#endif // SHOPITEM_H
