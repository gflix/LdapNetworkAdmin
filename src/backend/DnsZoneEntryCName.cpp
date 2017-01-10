/*
 * DnsZoneEntryCName.cpp
 *
 *  Created on: 10.01.2017
 *      Author: felix
 */

#include <backend/DnsZoneEntryCName.h>

namespace Flix {

DnsZoneEntryCName::DnsZoneEntryCName(const QString& description, const QString& host, const QString& target):
    GenericDnsZoneEntry(DnsZoneEntryType::CNAME, description),
    host(host),
    target(target)
{
}

DnsZoneEntryCName::~DnsZoneEntryCName()
{
}

QString DnsZoneEntryCName::getRecord(void) const
{
    return host + " IN CNAME " + target + '.';
}

} /* namespace Flix */
