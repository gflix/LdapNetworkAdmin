/*
 * TableModelConnections.h
 *
 *  Created on: 31.12.2016
 *      Author: felix
 */

#ifndef SRC_MODELCONNECTIONS_H_
#define SRC_MODELCONNECTIONS_H_

#include <QtCore/QAbstractListModel>
#include <Common.h>

namespace Flix {

class ModelConnections: public QAbstractListModel {
    Q_OBJECT

    Connections connections;
    void load(void);
    void save(void);
public:
    ModelConnections(QObject* parent);
    virtual ~ModelConnections();

    int rowCount(const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation = Qt::Horizontal, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

    void addConnection(const Connection& connection);
    const Connection& getConnection(const QModelIndex& index) const;
    void updateConnection(const QModelIndex& index, const Connection& connection);
    void deleteConnection(const QModelIndex& index);
};

} /* namespace Flix */

#endif /* SRC_MODELCONNECTIONS_H_ */
