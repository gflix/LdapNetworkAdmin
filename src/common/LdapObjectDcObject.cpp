/*
 * LdapObjectDcObject.cpp
 *
 *  Created on: 04.01.2017
 *      Author: felix
 */

#include <common/LdapObjectDcObject.h>
#include <common/LdapTags.h>

namespace Flix {

LdapObjectDcObject::LdapObjectDcObject():
    GenericLdapObject(LdapObjectType::DC_OBJECT)
{
}

LdapObjectDcObject::~LdapObjectDcObject()
{
}

bool LdapObjectDcObject::isValid(void) const
{
    const LdapAttributeValues& objectClasses = getAttribute(LDAP_ATTRIBUTE_OBJECT_CLASS);

    return
        !distinguishedName.isEmpty() &&
        objectClasses.contains(LDAP_OBJECT_CLASS_DC_OBJECT);
}

} /* namespace Flix */
