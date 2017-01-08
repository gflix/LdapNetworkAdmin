/*
 * GenericDnsZoneEntry.cpp
 *
 *  Created on: 08.01.2017
 *      Author: felix
 */

#include <backend/GenericDnsZoneEntry.h>

namespace Flix {

GenericDnsZoneEntry::GenericDnsZoneEntry(DnsZoneEntryType type):
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

} /* namespace Flix */
