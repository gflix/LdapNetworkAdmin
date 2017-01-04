/*
 * LdapObjectNetworkHost.cpp
 *
 *  Created on: 04.01.2017
 *      Author: felix
 */

#include <LdapObjectNetworkHost.h>
#include <LdapTags.h>

namespace Flix {

LdapObjectNetworkHost::LdapObjectNetworkHost():
        GenericLdapObject(LdapObjectType::NETWORK_HOST)
{
}

LdapObjectNetworkHost::~LdapObjectNetworkHost()
{
}

bool LdapObjectNetworkHost::isValid(void) const
{
    const LdapAttributeValues& objectClasses = getAttribute(LDAP_ATTRIBUTE_OBJECT_CLASS);

    return
        !distinguishedName.isEmpty() &&
        objectClasses.contains(LDAP_OBJECT_CLASS_DEVICE) &&
        objectClasses.contains(LDAP_OBJECT_CLASS_IP_HOST);
}

LdapObjectNetworkHost* LdapObjectNetworkHost::create(const QString& distinguishedName, const QString& ipAddress)
{
    LdapObjectNetworkHost* object = new LdapObjectNetworkHost();

    object->setDistinguishedName(distinguishedName);

    LdapAttributeValues values { LDAP_OBJECT_CLASS_TOP, LDAP_OBJECT_CLASS_DEVICE, LDAP_OBJECT_CLASS_IP_HOST };
    object->setAttribute(LDAP_ATTRIBUTE_OBJECT_CLASS, values);

    values = { ipAddress };
    object->setAttribute(LDAP_ATTRIBUTE_IP_HOST_NUMBER, values);

    return object;
}

} /* namespace Flix */
