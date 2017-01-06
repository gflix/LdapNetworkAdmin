/*
 * TableModelConnections.cpp
 *
 *  Created on: 31.12.2016
 *      Author: felix
 */

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QStandardPaths>
#include <QtCore/QTextStream>
#include <QtXml/QDomDocument>
#include <frontend/ModelConnections.h>

#define FILENAME_CONNECTIONS "LdapNetworkAdminConnections.xml"

#define XML_ELEMENT_CONNECTION "Connection"
#define XML_ELEMENT_CONNECTIONS "Connections"

#define XML_ATTRIBUTE_AUTH_DN "authDn"
#define XML_ATTRIBUTE_AUTH_PASSWORD "authPassword"
#define XML_ATTRIBUTE_BASE_DN "baseDn"
#define XML_ATTRIBUTE_HOST "host"
#define XML_ATTRIBUTE_NAME "name"
#define XML_ATTRIBUTE_PORT "port"
#define XML_ATTRIBUTE_SUB_OU "subOu"

namespace Flix {

ModelConnections::ModelConnections(QObject* parent):
    QAbstractListModel(parent)
{
    load();
}

ModelConnections::~ModelConnections()
{
}

int ModelConnections::rowCount(const QModelIndex&) const
{
    return connections.size();
}

int ModelConnections::columnCount(const QModelIndex&) const
{
    return 2;
}

QVariant ModelConnections::data(const QModelIndex& index, int role) const
{
    Q_ASSERT(index.row() < connections.size());
    auto connection = connections[index.row()];
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0:
            return connection.name;
            break;
        case 1:
            return QString("%1:%2").arg(connection.host).arg(connection.port);
            break;
        }
    }
    return QVariant();
}

QVariant ModelConnections::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return tr("Connection");
            break;
        case 1:
            return tr("Host");
            break;
        default:
            break;
        }
    }
    return QVariant();
}

void ModelConnections::addConnection(const Connection& connection)
{
    connections.push_back(connection);
    save();

    emit layoutChanged();
}

const Connection& ModelConnections::getConnection(const QModelIndex& index) const
{
    Q_ASSERT(index.isValid() && index.row() < connections.size());
    return connections[index.row()];
}

void ModelConnections::updateConnection(const QModelIndex& index, const Connection& connection)
{
    if (!index.isValid() || index.row() >= connections.size()) {
        return;
    }
    connections.replace(index.row(), connection);
    save();

    emit layoutChanged();
}

void ModelConnections::deleteConnection(const QModelIndex& index)
{
    if (!index.isValid() || index.row() >= connections.size()) {
        return;
    }
    connections.removeAt(index.row());
    save();

    emit layoutChanged();
}

void ModelConnections::load(void)
{
    QFile fileConnections(QStandardPaths::locate(QStandardPaths::ConfigLocation, FILENAME_CONNECTIONS));
    connections.clear();
    if (!fileConnections.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QDomDocument domConnections;
    if (!domConnections.setContent(&fileConnections)) {
        return;
    }
    QDomElement elementConnections = domConnections.documentElement();
    if (elementConnections.tagName() != XML_ELEMENT_CONNECTIONS) {
        return;
    }
    for (auto elementConnection = elementConnections.firstChildElement(XML_ELEMENT_CONNECTION); !elementConnection.isNull();
        elementConnection = elementConnection.nextSiblingElement(XML_ELEMENT_CONNECTION)) {
        Connection connection;

        connection.name = elementConnection.attribute(XML_ATTRIBUTE_NAME);
        connection.host = elementConnection.attribute(XML_ATTRIBUTE_HOST);
        connection.port = elementConnection.attribute(XML_ATTRIBUTE_PORT, "-1").toInt(nullptr, 10);
        connection.baseDn = elementConnection.attribute(XML_ATTRIBUTE_BASE_DN);
        connection.authDn = elementConnection.attribute(XML_ATTRIBUTE_AUTH_DN);
        connection.authPassword = elementConnection.attribute(XML_ATTRIBUTE_AUTH_PASSWORD);
        connection.savePassword = !connection.authPassword.isEmpty();
        connection.subOu = elementConnection.attribute(XML_ATTRIBUTE_SUB_OU);

        connections.push_back(connection);
    }
}

void ModelConnections::save(void)
{
    QDir dirConnections { QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) };
    QFile fileConnections(dirConnections.filePath(FILENAME_CONNECTIONS));

    if (!fileConnections.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }

    QDomDocument domConnections;
    QDomProcessingInstruction processingInstruction =
        domConnections.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\"");
    domConnections.appendChild(processingInstruction);

    QDomElement elementConnections = domConnections.createElement(XML_ELEMENT_CONNECTIONS);
    for (auto& connection: connections) {
        QDomElement elementConnection = domConnections.createElement(XML_ELEMENT_CONNECTION);
        elementConnection.setAttribute(XML_ATTRIBUTE_NAME, connection.name);
        elementConnection.setAttribute(XML_ATTRIBUTE_HOST, connection.host);
        elementConnection.setAttribute(XML_ATTRIBUTE_PORT, connection.port);
        elementConnection.setAttribute(XML_ATTRIBUTE_BASE_DN, connection.baseDn);
        elementConnection.setAttribute(XML_ATTRIBUTE_AUTH_DN, connection.authDn);
        if (connection.savePassword) {
            elementConnection.setAttribute(XML_ATTRIBUTE_AUTH_PASSWORD, connection.authPassword);
        }
        elementConnection.setAttribute(XML_ATTRIBUTE_SUB_OU, connection.subOu);

        elementConnections.appendChild(elementConnection);
    }
    domConnections.appendChild(elementConnections);

    QTextStream streamConnections(&fileConnections);
    domConnections.save(streamConnections, 2);
}

} /* namespace Flix */
