/*
 * ModelNetworkTree.h
 *
 *  Created on: 02.01.2017
 *      Author: felix
 */

#ifndef SRC_MODELNETWORKTREE_H_
#define SRC_MODELNETWORKTREE_H_

#include <QtCore/QAbstractItemModel>
#include <common/Common.h>

namespace Flix {

class ModelNetworkTree: public QAbstractItemModel {
    NetworkTreeItem* root;
public:
    ModelNetworkTree(QObject* parent = 0);
    virtual ~ModelNetworkTree();

    void clear(void);

    QVariant data(const QModelIndex& index, int role) const Q_DECL_OVERRIDE;

    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex& index) const Q_DECL_OVERRIDE;
    NetworkTreeItem *getItem(const QModelIndex& index) const;

    int rowCount(const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;
    bool hasChildren(const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;

    bool addChild(GenericLdapObject* object, const QModelIndex& parent);
    void deleteTree(const QModelIndex& parent);
    void updateItem(GenericLdapObject* object, const QModelIndex& index);
};

} /* namespace Flix */

#endif /* SRC_MODELNETWORKTREE_H_ */
