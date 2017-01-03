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

QString LdapObject::getShortName(void) const
{
    QString shortenedName { distinguishedName };
    int separatorPosition = distinguishedName.indexOf(',');
    if (separatorPosition >= 0) {
        shortenedName = shortenedName.mid(0, separatorPosition);
    }
    separatorPosition = shortenedName.indexOf('=');
    if (separatorPosition >= 0) {
        shortenedName = shortenedName.mid(separatorPosition + 1);
    }
    return shortenedName;
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

LdapObject LdapObject::createOrganizationalUnit(const QString& distinguishedName)
{
    LdapObject object;

    object.setDistinguishedName(distinguishedName);
    LdapAttributeValues values { LDAP_OBJECT_CLASS_TOP, LDAP_OBJECT_CLASS_ORGANIZATIONAL_UNIT };
    object.setAttribute(LDAP_ATTRIBUTE_OBJECT_CLASS, values);

    return object;
}

} /* namespace Flix */
