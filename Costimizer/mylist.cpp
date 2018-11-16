#include "mylist.h"

MyList::MyList(QObject *parent)
    : QAbstractListModel(parent)
{
}

void MyList::clear()
{
    beginResetModel();
    this->myStrings.clear();
    endResetModel();
}

void MyList::addItem( const QString &item )
{
    int row = this->rowCount();
    this->insertRows(row,1);

    QModelIndex index = this->index(row);
    this->setData(index, item);
}

int MyList::rowCount(const QModelIndex &parent) const
{
    return this->myStrings.size();
}

QVariant MyList::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= this->myStrings.size())
        return QVariant();

    if (role == Qt::DisplayRole)
        return this->myStrings.at(index.row());
    else
        return QVariant();
}

bool MyList::setData(const QModelIndex &index, const QVariant &value, int role)
{
    auto ret = index.isValid();
    if (index.isValid() && role == Qt::EditRole) {

        this->myStrings[index.row()] = value.toString();
        emit dataChanged(index, index);
        return true;
    }

    return false;
}

bool MyList::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);

    for( int i = 0; i < count; i++ )
    {
        this->myStrings.insert(row, "");
    }

    endInsertRows();

    return true;
}

bool MyList::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);

    for( int i = 0; i < count; i++ )
    {
        this->myStrings.removeAt(row);
    }

    endRemoveRows();
    return true;
}
