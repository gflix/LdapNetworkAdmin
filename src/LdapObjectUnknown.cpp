/*
 * LdapObjectUnknown.cpp
 *
 *  Created on: 05.01.2017
 *      Author: felix
 */

#include <LdapObjectUnknown.h>

namespace Flix {

LdapObjectUnknown::LdapObjectUnknown():
    GenericLdapObject(LdapObjectType::UNKNOWN)
{
}

LdapObjectUnknown::~LdapObjectUnknown()
{
}

bool LdapObjectUnknown::isValid(void) const
{
    return !distinguishedName.isEmpty();
}

} /* namespace Flix */
