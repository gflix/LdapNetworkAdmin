/*
 * LdapObject.cpp
 *
 *  Created on: 02.01.2017
 *      Author: felix
 */

#include <LdapObject.h>
#include <LdapTags.h>

namespace Flix {

LdapObject::LdapObject()
{
}

LdapObject::~LdapObject()
{
}

void LdapObject::clear(void)
{
    setDistinguishedName(QString());
    attributes.clear();
}

void LdapObject::setDistinguishedName(const QString& distinguishedName)
{
    this->distinguishedName = distinguishedName;
}

void LdapObject::setAttribute(const QString& attribute, const LdapAttributeValues& values)
{
    attributes[attribute] = values;
}

bool LdapObject::isValid(void) const
{
    return !distinguishedName.isEmpty() && hasAttribute(LDAP_ATTRIBUTE_OBJECT_CLASS);
}

const QString& LdapObject::getDistinguishedName(void) const
{
    return distinguishedName;
}

QString LdapObject::getIdentifier(void) const
{
    QRegExp regexCurrentIdentifier { R"re(^\w+=([\w\.-]+),.+$)re" };
    if (regexCurrentIdentifier.exactMatch(distinguishedName)) {
        return regexCurrentIdentifier.cap(1);
    }

    return distinguishedName;
}

LdapAttributes LdapObject::getAttributes(void) const
{
    return attributes.keys();
}

bool LdapObject::hasAttribute(const QString& attribute) const
{
    return attributes.contains(attribute);
}

LdapAttributeValues LdapObject::getAttribute(const QString& attribute) const
{
    if (!attributes.contains(attribute)) {
        return { };
    }
    return attributes[attribute];
}

bool LdapObject::isDcObject(void) const
{
    return getAttribute(LDAP_ATTRIBUTE_OBJECT_CLASS).contains(LDAP_OBJECT_CLASS_DC_OBJECT);
}

bool LdapObject::isOrganizationalUnit(void) const
{
    return getAttribute(LDAP_ATTRIBUTE_OBJECT_CLASS).contains(LDAP_OBJECT_CLASS_ORGANIZATIONAL_UNIT);
}

bool LdapObject::isNetworkHost(void) const
{
    const LdapAttributeValues& objectClasses = getAttribute(LDAP_ATTRIBUTE_OBJECT_CLASS);
    return objectClasses.contains(LDAP_OBJECT_CLASS_DEVICE) && objectClasses.contains(LDAP_OBJECT_CLASS_IP_HOST);
}

LdapObject LdapObject::createOrganizationalUnit(const QString& distinguishedName)
{
    LdapObject object;

    object.setDistinguishedName(distinguishedName);

    LdapAttributeValues values { LDAP_OBJECT_CLASS_TOP, LDAP_OBJECT_CLASS_ORGANIZATIONAL_UNIT };
    object.setAttribute(LDAP_ATTRIBUTE_OBJECT_CLASS, values);

    return object;
}

LdapObject LdapObject::createNetworkHost(const QString& distinguishedName)
{
    LdapObject object;

    object.setDistinguishedName(distinguishedName);

    LdapAttributeValues values { LDAP_OBJECT_CLASS_TOP, LDAP_OBJECT_CLASS_DEVICE, LDAP_OBJECT_CLASS_IP_HOST };
    object.setAttribute(LDAP_ATTRIBUTE_OBJECT_CLASS, values);

    values = { "0.0.0.0" };
    object.setAttribute(LDAP_ATTRIBUTE_IP_HOST_NUMBER, values);

    return object;
}

} /* namespace Flix */
