/*
 * Common.cpp
 *
 *  Created on: 01.01.2017
 *      Author: felix
 */

#include <common/Common.h>

namespace Flix {

Connection::Connection(void):
    port(0),
    anonymousBind(false),
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

NetworkTreeItem::NetworkTreeItem(GenericLdapObject* object, NetworkTreeItem* parent):
    parent(parent),
    object(object)
{
}

NetworkTreeItem::~NetworkTreeItem(void)
{
    clearChildren();
    if (object) {
        delete object;
    }
}

void NetworkTreeItem::clearChildren(void)
{
    qDeleteAll(children);
    children.clear();
}

void NetworkTreeItem::addChild(GenericLdapObject* object)
{
    children.push_back(new NetworkTreeItem(object, this));
}

void NetworkTreeItem::setObject(GenericLdapObject* object)
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

GenericLdapObject* NetworkTreeItem::getObject(void) const
{
    return object;
}

} /* namespace Flix */
