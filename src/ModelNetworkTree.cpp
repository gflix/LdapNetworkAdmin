/*
 * ModelNetworkTree.cpp
 *
 *  Created on: 02.01.2017
 *      Author: felix
 */

#include <ModelNetworkTree.h>
#include <LdapTags.h>

namespace Flix {

ModelNetworkTree::ModelNetworkTree(QObject *parent):
    QAbstractItemModel(parent)
{
    root = new NetworkTreeItem(LdapObject());
}

ModelNetworkTree::~ModelNetworkTree()
{
    delete root;
}

void ModelNetworkTree::clear(void)
{
    int rowCount = root->getChildCount();
    if (rowCount > 0) {
        beginRemoveRows(QModelIndex(), 0, rowCount);
        root->clearChildren();
        endRemoveRows();
    }
}

QVariant ModelNetworkTree::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }
    NetworkTreeItem* item = getItem(index);
    if (!item) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        return item->getObject().getShortName();
    }
    return QVariant();
}

QModelIndex ModelNetworkTree::index(int row, int column, const QModelIndex& parent) const
{
    if (parent.isValid() && parent.column() != 0) {
        return QModelIndex();
    }

    NetworkTreeItem* child = getItem(parent)->getChild(row);
    if (child) {
        return createIndex(row, column, child);
    }

    return QModelIndex();
}

QModelIndex ModelNetworkTree::parent(const QModelIndex& index) const
{
    if (!index.isValid()) {
        return QModelIndex();
    }

    NetworkTreeItem* indexParent = getItem(index)->getParent();
    if (indexParent == root) {
        return QModelIndex();
    }

    return createIndex(indexParent->getChildNumber(), 0, indexParent);
}

int ModelNetworkTree::rowCount(const QModelIndex& parent) const
{
    return getItem(parent)->getChildCount();
}

int ModelNetworkTree::columnCount(const QModelIndex& parent) const
{
    return 1;
}

bool ModelNetworkTree::hasChildren(const QModelIndex& parent) const
{
    NetworkTreeItem* item = getItem(parent);
    if (!item) {
        return false;
    }
    // the root item is not visible but needs to have children
    if (item == root) {
        return true;
    }
    return item->isContainerObject();
}

bool ModelNetworkTree::addChild(const LdapObject& object, const QModelIndex &parent)
{
    NetworkTreeItem* item = getItem(parent);
    if (!item) {
        return false;
    }
    item->addChild(object);
    emit layoutChanged();
    return true;
}

void ModelNetworkTree::deleteTree(const QModelIndex& parent)
{
    NetworkTreeItem* item = getItem(parent);
    if (!item) {
        return;
    }
    int rowCount = item->getChildCount();
    if (rowCount > 0) {
        beginRemoveRows(parent, 0, rowCount);
        item->clearChildren();
        endRemoveRows();
    }
}

NetworkTreeItem* ModelNetworkTree::getItem(const QModelIndex& index) const
{
    if (index.isValid()) {
        NetworkTreeItem* item = static_cast<NetworkTreeItem*>(index.internalPointer());
        if (item) {
            return item;
        }
    }
    return root;
}

} /* namespace Flix */
