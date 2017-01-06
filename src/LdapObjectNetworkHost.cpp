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
    QString ipHostNumber;
    if (!getSingleAttribute(LDAP_ATTRIBUTE_IP_HOST_NUMBER, ipHostNumber)) {
        return QString();
    }
    return ipHostNumber;
}

QString LdapObjectNetworkHost::getDescription(void) const
{
    QString description;
    if (!getSingleAttribute(LDAP_ATTRIBUTE_DESCRIPTION, description)) {
        return QString();
    }
    return description;
}

QString LdapObjectNetworkHost::getMacAddress(void) const
{
    QString macAddress;
    if (!getSingleAttribute(LDAP_ATTRIBUTE_MAC_ADDRESS, macAddress)) {
        return QString();
    }
    return macAddress;
}

void LdapObjectNetworkHost::setIpAddress(const QString& ipAddress)
{
    setSingleAttribute(LDAP_ATTRIBUTE_IP_HOST_NUMBER, ipAddress);
}

void LdapObjectNetworkHost::setDescription(const QString& description)
{
    setSingleAttribute(LDAP_ATTRIBUTE_DESCRIPTION, description);
}

void LdapObjectNetworkHost::setMacAddress(const QString& macAddress)
{
    setSingleAttribute(LDAP_ATTRIBUTE_MAC_ADDRESS, macAddress);}

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
