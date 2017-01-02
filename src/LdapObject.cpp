/*
 * LdapObject.cpp
 *
 *  Created on: 02.01.2017
 *      Author: felix
 */

#include <LdapObject.h>

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
    return !distinguishedName.isEmpty() && hasAttribute("objectClass");
}

const QString& LdapObject::getDistinguishedName(void) const
{
    return distinguishedName;
}

bool LdapObject::hasAttribute(const QString& attribute) const
{
    return attributes.contains(attribute);
}

} /* namespace Flix */
