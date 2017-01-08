/*
 * DnsZoneEntryPtr.h
 *
 *  Created on: 08.01.2017
 *      Author: felix
 */

#ifndef SRC_BACKEND_DNSZONEENTRYPTR_H_
#define SRC_BACKEND_DNSZONEENTRYPTR_H_

#include <backend/GenericDnsZoneEntry.h>

namespace Flix {

class DnsZoneEntryPtr: public GenericDnsZoneEntry {
public:
    DnsZoneEntryPtr(const QString& description, const QString& fullyQualifiedHostname, const QString& ipAddressPart);
    virtual ~DnsZoneEntryPtr();

    const QString& getFullyQualifiedHostname(void) const;
    const QString& getIpAddressPart(void) const;
    virtual QString getRecord(void) const;

private:
    QString fullyQualifiedHostname;
    QString ipAddressPart;
};

} /* namespace Flix */

#endif /* SRC_BACKEND_DNSZONEENTRYPTR_H_ */
