/*
 * DnsZoneEntryPtr.cpp
 *
 *  Created on: 08.01.2017
 *      Author: felix
 */

#include <backend/DnsZoneEntryPtr.h>

namespace Flix {

DnsZoneEntryPtr::DnsZoneEntryPtr(const QString& fullyQualifiedHostname, const QString& ipAddressPart):
    GenericDnsZoneEntry(DnsZoneEntryType::PTR),
    fullyQualifiedHostname(fullyQualifiedHostname),
    ipAddressPart(ipAddressPart)
{
}

DnsZoneEntryPtr::~DnsZoneEntryPtr()
{
}

const QString& DnsZoneEntryPtr::getFullyQualifiedHostname(void) const
{
    return fullyQualifiedHostname;
}

const QString& DnsZoneEntryPtr::getIpAddressPart(void) const
{
    return ipAddressPart;
}

QString DnsZoneEntryPtr::getRecord(void) const
{
    return ipAddressPart + " IN PTR " + fullyQualifiedHostname + '.';
}

} /* namespace Flix */
