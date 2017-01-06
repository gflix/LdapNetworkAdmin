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

QString LdapObjectNetworkHost::getIpAddress(void) const
{
    const LdapAttributeValues& ipHostNumbers = getAttribute(LDAP_ATTRIBUTE_IP_HOST_NUMBER);

    if (!ipHostNumbers.empty()) {
        return ipHostNumbers[0];
    }
    return QString();
}

QString LdapObjectNetworkHost::getMacAddress(void) const
{
    const LdapAttributeValues& macAddresses = getAttribute(LDAP_ATTRIBUTE_MAC_ADDRESS);

    if (!macAddresses.empty()) {
        return macAddresses[0];
    }
    return QString();
}

void LdapObjectNetworkHost::setIpAddress(const QString& ipAddress)
{
    LdapAttributeValues ipHostNumbers { ipAddress };
    setAttribute(LDAP_ATTRIBUTE_IP_HOST_NUMBER, ipHostNumbers);
}

void LdapObjectNetworkHost::setMacAddress(const QString& macAddress)
{
    LdapAttributeValues macAddresses { macAddress };
    setAttribute(LDAP_ATTRIBUTE_MAC_ADDRESS, macAddresses);
}

LdapObjectNetworkHost* LdapObjectNetworkHost::create(const QString& distinguishedName, const QString& ipAddress)
{
    LdapObjectNetworkHost* object = new LdapObjectNetworkHost();

    object->setDistinguishedName(distinguishedName);

    LdapAttributeValues values { LDAP_OBJECT_CLASS_TOP, LDAP_OBJECT_CLASS_DEVICE, LDAP_OBJECT_CLASS_IP_HOST, LDAP_OBJECT_CLASS_IEEE802_DEVICE };
    object->setAttribute(LDAP_ATTRIBUTE_OBJECT_CLASS, values);

    values = { ipAddress };
    object->setAttribute(LDAP_ATTRIBUTE_IP_HOST_NUMBER, values);

    return object;
}

} /* namespace Flix */
