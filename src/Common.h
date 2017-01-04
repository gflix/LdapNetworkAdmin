/*
 * Common.h
 *
 *  Created on: 01.01.2017
 *      Author: felix
 */

#ifndef SRC_COMMON_H_
#define SRC_COMMON_H_

#include <QtCore/QList>
#include <QtCore/QString>
#include <LdapObject.h>

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

    LdapObject object;
public:
    NetworkTreeItem(const LdapObject& object, NetworkTreeItem* parent = nullptr);
    virtual ~NetworkTreeItem();

    void clearChildren(void);
    void addChild(const LdapObject& object);

    NetworkTreeItem* getChild(int number);
    int getChildCount(void) const;
    int getChildNumber(void) const;
    NetworkTreeItem* getParent(void) const;
    const LdapObject& getObject(void) const;
};


typedef QList<QString> LdapDistinguishedNameParts;

QString joinDistinguishedName(const LdapDistinguishedNameParts& parts);

} /* namespace Flix */

#endif /* SRC_COMMON_H_ */
