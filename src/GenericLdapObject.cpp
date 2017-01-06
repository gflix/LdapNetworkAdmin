/*
 * LdapObject.cpp
 *
 *  Created on: 02.01.2017
 *      Author: felix
 */

#include <GenericLdapObject.h>
#include <LdapObjectDcObject.h>
#include <LdapObjectOrganizationalUnit.h>
#include <LdapObjectNetworkHost.h>
#include <LdapObjectUnknown.h>
#include <LdapTags.h>

namespace Flix {

GenericLdapObject::GenericLdapObject(LdapObjectType type):
    type(type)
{
}

GenericLdapObject::~GenericLdapObject()
{
}

void GenericLdapObject::clear(void)
{
    setDistinguishedName(QString());
    attributes.clear();
}

void GenericLdapObject::setDistinguishedName(const QString& distinguishedName)
{
    this->distinguishedName = distinguishedName;
}

void GenericLdapObject::setAttribute(const QString& attribute, const LdapAttributeValues& values)
{
    attributes[attribute] = values;
}

LdapObjectType GenericLdapObject::getType(void) const
{
    if (!isValid()) {
        return LdapObjectType::UNKNOWN;
    }
    return type;
}

bool GenericLdapObject::isObjectType(LdapObjectType type) const
{
    return this->type == type && isValid();
}

const QString& GenericLdapObject::getDistinguishedName(void) const
{
    return distinguishedName;
}

QString GenericLdapObject::getIdentifier(void) const
{
    QRegExp regexCurrentIdentifier { R"re(^\w+=([\w\.-]+),.+$)re" };
    if (regexCurrentIdentifier.exactMatch(distinguishedName)) {
        return regexCurrentIdentifier.cap(1);
    }

    return distinguishedName;
}

LdapAttributes GenericLdapObject::getAttributes(void) const
{
    return attributes.keys();
}

bool GenericLdapObject::hasAttribute(const QString& attribute) const
{
    return attributes.contains(attribute);
}

LdapAttributeValues GenericLdapObject::getAttribute(const QString& attribute) const
{
    if (!attributes.contains(attribute)) {
        return { };
    }
    return attributes[attribute];
}

bool GenericLdapObject::getSingleAttribute(const QString& attribute, QString& attributeValue) const
{
    const LdapAttributeValues& values = getAttribute(attribute);

    if (values.empty()) {
        return false;
    }
    attributeValue = values[0];
    return true;
}

void GenericLdapObject::setSingleAttribute(const QString& attribute, const QString& attributeValue)
{
    LdapAttributeValues values { attributeValue };
    setAttribute(attribute, values);
}

GenericLdapObject* GenericLdapObject::getDuplicate(const GenericLdapObject* original)
{
    GenericLdapObject* duplicate = nullptr;
    if (!original) {
        return duplicate;
    }
    switch (original->getType()) {
    case LdapObjectType::UNKNOWN:
        duplicate = new LdapObjectUnknown(*((LdapObjectUnknown*) original));
        break;
    case LdapObjectType::DC_OBJECT:
        duplicate = new LdapObjectDcObject(*((LdapObjectDcObject*) original));
        break;
    case LdapObjectType::ORGANIZATIONAL_UNIT:
        duplicate = new LdapObjectOrganizationalUnit(*((LdapObjectOrganizationalUnit*) original));
        break;
    case LdapObjectType::NETWORK_HOST:
        duplicate = new LdapObjectNetworkHost(*((LdapObjectNetworkHost*) original));
        break;
    default:
        break;
    }
    return duplicate;
}

} /* namespace Flix */
