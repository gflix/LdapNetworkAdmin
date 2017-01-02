/*
 * ModelNetworkTree.h
 *
 *  Created on: 02.01.2017
 *      Author: felix
 */

#ifndef SRC_MODELNETWORKTREE_H_
#define SRC_MODELNETWORKTREE_H_

#include <QtCore/qabstractitemmodel.h>

namespace Flix {

class ModelNetworkTree: public QAbstractItemModel {
public:
    ModelNetworkTree(QObject *parent = 0);
    virtual ~ModelNetworkTree();

    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
};

} /* namespace Flix */

#endif /* SRC_MODELNETWORKTREE_H_ */
