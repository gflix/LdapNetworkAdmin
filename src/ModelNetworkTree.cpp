/*
 * ModelNetworkTree.cpp
 *
 *  Created on: 02.01.2017
 *      Author: felix
 */

#include <ModelNetworkTree.h>

namespace Flix {

ModelNetworkTree::ModelNetworkTree(QObject *parent):
    QAbstractItemModel(parent)
{
}

ModelNetworkTree::~ModelNetworkTree()
{
}

QVariant ModelNetworkTree::data(const QModelIndex &index, int role) const
{
    // TODO: retrieve real data
    return QVariant();
}

QModelIndex ModelNetworkTree::index(int row, int column, const QModelIndex& parent) const
{
    if (parent.isValid() && parent.column() != 0) {
        return QModelIndex();
    }

    // TODO: retrieve real index
    return QModelIndex();
}

QModelIndex ModelNetworkTree::parent(const QModelIndex& index) const
{
    if (!index.isValid()) {
        return QModelIndex();
    }

    // TODO: retrieve real parent
    return QModelIndex();
}

int ModelNetworkTree::rowCount(const QModelIndex& parent) const
{
    // TODO: retrieve real rowCount
    return 0;
}

int ModelNetworkTree::columnCount(const QModelIndex& parent) const
{
    // TODO: retrieve real columnCount
    return 1;
}

} /* namespace Flix */
