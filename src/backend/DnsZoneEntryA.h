/*
 * DnsZoneEntryA.h
 *
 *  Created on: 08.01.2017
 *      Author: felix
 */

#ifndef SRC_BACKEND_DNSZONEENTRYA_H_
#define SRC_BACKEND_DNSZONEENTRYA_H_

#include <backend/GenericDnsZoneEntry.h>

namespace Flix {

class DnsZoneEntryA: public GenericDnsZoneEntry {
public:
    DnsZoneEntryA(const QString& description, const QString& host, const QString& ipAddress);
    virtual ~DnsZoneEntryA();

    const QString& getHost(void) const;
    const QString& getIpAddress(void) const;
    virtual QString getRecord(void) const;

private:
    QString host;
    QString ipAddress;
};

} /* namespace Flix */

#endif /* SRC_BACKEND_DNSZONEENTRYA_H_ */
