/*
 * Common.cpp
 *
 *  Created on: 01.01.2017
 *      Author: felix
 */

#include <Common.h>

namespace Flix {

Connection::Connection(void):
    port(0),
    savePassword(false)
{
}

QString joinDistinguishedName(const LdapDistinguishedNameParts& parts)
{
    QString distinguishedName;
    int index = 0;
    for (auto part: parts) {
        if (part.isEmpty()) {
            continue;
        }
        if (index > 0) {
            distinguishedName = ',' + distinguishedName;
        }
        distinguishedName = part + distinguishedName;
        ++index;
    }
    return distinguishedName;
}

NetworkTreeItem::NetworkTreeItem(const LdapObject& object, NetworkTreeItem* parent):
    parent(parent),
    object(object)
{
}

NetworkTreeItem::~NetworkTreeItem(void)
{
    clearChildren();
}

void NetworkTreeItem::clearChildren(void)
{
    qDeleteAll(children);
    children.clear();
}

void NetworkTreeItem::addChild(const LdapObject& object)
{
    children.push_back(new NetworkTreeItem(object, this));
}

void NetworkTreeItem::setObject(const LdapObject& object)
{
    this->object = object;
}

NetworkTreeItem* NetworkTreeItem::getChild(int number)
{
    Q_ASSERT(number >= 0 && number < children.size());
    return children.value(number);
}

int NetworkTreeItem::getChildCount(void) const
{
    return children.size();
}

int NetworkTreeItem::getChildNumber(void) const
{
    if (parent) {
        return parent->children.indexOf(const_cast<NetworkTreeItem*>(this));
    }

    return 0;
}

NetworkTreeItem* NetworkTreeItem::getParent(void) const
{
    return parent;
}

const LdapObject& NetworkTreeItem::getObject(void) const
{
    return object;
}

} /* namespace Flix */
