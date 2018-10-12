#ifndef MYLIST_H
#define MYLIST_H

#include <QAbstractListModel>
#include <QList>

class MyList : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit MyList( QObject *parent = nullptr );

    void clear();
    void addItem( const QString &item );

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

private:
    QList<QString> myStrings;
};

#endif // MYLIST_H
