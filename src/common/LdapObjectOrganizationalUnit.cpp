/*
 * LdapObjectOrganizationalUnit.cpp
 *
 *  Created on: 04.01.2017
 *      Author: felix
 */

#include <common/LdapObjectOrganizationalUnit.h>
#include <common/LdapTags.h>

namespace Flix {

LdapObjectOrganizationalUnit::LdapObjectOrganizationalUnit():
    GenericLdapObject(LdapObjectType::ORGANIZATIONAL_UNIT)
{
}

LdapObjectOrganizationalUnit::~LdapObjectOrganizationalUnit()
{
}

bool LdapObjectOrganizationalUnit::isValid(void) const
{
    const LdapAttributeValues& objectClasses = getAttribute(LDAP_ATTRIBUTE_OBJECT_CLASS);

    return
        !distinguishedName.isEmpty() &&
        objectClasses.contains(LDAP_OBJECT_CLASS_ORGANIZATIONAL_UNIT);
}

LdapObjectOrganizationalUnit* LdapObjectOrganizationalUnit::create(const QString& distinguishedName)
{
    LdapObjectOrganizationalUnit* object = new LdapObjectOrganizationalUnit();

    object->setDistinguishedName(distinguishedName);

    LdapAttributeValues values { LDAP_OBJECT_CLASS_TOP, LDAP_OBJECT_CLASS_ORGANIZATIONAL_UNIT };
    object->setAttribute(LDAP_ATTRIBUTE_OBJECT_CLASS, values);

    return object;
}

} /* namespace Flix */
