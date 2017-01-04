/*
 * Common.h
 *
 *  Created on: 01.01.2017
 *      Author: felix
 */

#ifndef SRC_COMMON_H_
#define SRC_COMMON_H_

#include <GenericLdapObject.h>
#include <QtCore/QList>
#include <QtCore/QString>

namespace Flix {

struct Connection {
    Connection();

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

class NetworkTreeItem;
typedef QList<NetworkTreeItem*> NetworkTreeItems;

class NetworkTreeItem {
    NetworkTreeItem* parent;
    NetworkTreeItems children;

    const GenericLdapObject* object;
public:
    NetworkTreeItem(const GenericLdapObject* object, NetworkTreeItem* parent = nullptr);
    virtual ~NetworkTreeItem();

    void clearChildren(void);
    void addChild(const GenericLdapObject* object);
    void setObject(const GenericLdapObject* object);

    NetworkTreeItem* getChild(int number);
    int getChildCount(void) const;
    int getChildNumber(void) const;
    NetworkTreeItem* getParent(void) const;
    const GenericLdapObject* getObject(void) const;
};


typedef QList<QString> LdapDistinguishedNameParts;

QString joinDistinguishedName(const LdapDistinguishedNameParts& parts);

} /* namespace Flix */

#endif /* SRC_COMMON_H_ */
