/*
 * DnsZoneEntryCName.h
 *
 *  Created on: 10.01.2017
 *      Author: felix
 */

#ifndef SRC_BACKEND_DNSZONEENTRYCNAME_H_
#define SRC_BACKEND_DNSZONEENTRYCNAME_H_

#include <backend/GenericDnsZoneEntry.h>

namespace Flix {

class DnsZoneEntryCName: public GenericDnsZoneEntry {
public:
    DnsZoneEntryCName(const QString& description, const QString& host, const QString& target);
    virtual ~DnsZoneEntryCName();

    virtual QString getRecord(void) const;

private:
    QString host;
    QString target;
};

} /* namespace Flix */

#endif /* SRC_BACKEND_DNSZONEENTRYCNAME_H_ */
