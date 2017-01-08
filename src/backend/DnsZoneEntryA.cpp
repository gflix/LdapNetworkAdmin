/*
 * DnsZoneEntryA.cpp
 *
 *  Created on: 08.01.2017
 *      Author: felix
 */

#include <backend/DnsZoneEntryA.h>

namespace Flix {

DnsZoneEntryA::DnsZoneEntryA(const QString& host, const QString& ipAddress):
    GenericDnsZoneEntry(DnsZoneEntryType::A),
    host(host),
    ipAddress(ipAddress)
{
}

DnsZoneEntryA::~DnsZoneEntryA()
{
}

const QString& DnsZoneEntryA::getHost(void) const
{
    return host;
}

const QString& DnsZoneEntryA::getIpAddress(void) const
{
    return ipAddress;
}

QString DnsZoneEntryA::getRecord(void) const
{
    return host + " IN A " + ipAddress;
}

} /* namespace Flix */
