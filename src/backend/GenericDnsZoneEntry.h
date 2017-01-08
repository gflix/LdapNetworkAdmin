/*
 * GenericDnsZoneEntry.h
 *
 *  Created on: 08.01.2017
 *      Author: felix
 */

#ifndef SRC_BACKEND_GENERICDNSZONEENTRY_H_
#define SRC_BACKEND_GENERICDNSZONEENTRY_H_

#include <memory>
#include <QtCore/QMap>
#include <QtCore/QList>
#include <QtCore/QString>

namespace Flix {

enum class DnsZoneEntryType {
    UNKNOWN,
    A,
    CNAME,
    PTR,
};

class GenericDnsZoneEntry {
public:
    GenericDnsZoneEntry(DnsZoneEntryType type);
    virtual ~GenericDnsZoneEntry();

    DnsZoneEntryType getType(void) const;
    virtual QString getRecord(void) const = 0;

private:
    DnsZoneEntryType type;
};

typedef std::shared_ptr<GenericDnsZoneEntry> DnsZoneEntry;
typedef QList<DnsZoneEntry> DnsZoneEntries;
typedef QMap<QString, DnsZoneEntries> DnsZones;

} /* namespace Flix */

#endif /* SRC_BACKEND_GENERICDNSZONEENTRY_H_ */
