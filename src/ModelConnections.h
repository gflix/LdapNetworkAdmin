/*
 * TableModelConnections.h
 *
 *  Created on: 31.12.2016
 *      Author: felix
 */

#ifndef SRC_MODELCONNECTIONS_H_
#define SRC_MODELCONNECTIONS_H_

#include <QtCore/QAbstractListModel>
#include <QtCore/QList>

namespace Flix {

struct Connection
{
    Connection();

//    int index;
    QString name;
    QString host;
    int port;
    QString baseDn;
    QString authDn;
    QString authPassword;
    bool savePassword;
    QString subOu;
};

typedef QList<Connection> Connections;

class ModelConnections: public QAbstractListModel {
    Q_OBJECT

    Connections connections;
//    int nextConnectionIndex;
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
