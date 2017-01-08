/*
 * DhcpHost.h
 *
 *  Created on: 08.01.2017
 *      Author: felix
 */

#ifndef SRC_BACKEND_DHCPHOST_H_
#define SRC_BACKEND_DHCPHOST_H_

#include <QtCore/QList>
#include <QtCore/QString>

namespace Flix {

struct DhcpHost {
    QString fullyQualifiedHostname;
    QString description;
    QString macAddress;
};

typedef QList<DhcpHost> DhcpHosts;

} /* namespace Flix */

#endif /* SRC_BACKEND_DHCPHOST_H_ */
