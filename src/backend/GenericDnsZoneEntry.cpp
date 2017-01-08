/*
 * GenericDnsZoneEntry.cpp
 *
 *  Created on: 08.01.2017
 *      Author: felix
 */

#include <backend/GenericDnsZoneEntry.h>

namespace Flix {

GenericDnsZoneEntry::GenericDnsZoneEntry(DnsZoneEntryType type, const QString& description):
    description(description),
    type(type)
{
}

GenericDnsZoneEntry::~GenericDnsZoneEntry()
{
}

DnsZoneEntryType GenericDnsZoneEntry::getType(void) const
{
    return type;
}

const QString& GenericDnsZoneEntry::getDescription(void) const
{
    return description;
}

} /* namespace Flix */
