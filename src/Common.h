/*
 * Common.h
 *
 *  Created on: 01.01.2017
 *      Author: felix
 */

#ifndef SRC_COMMON_H_
#define SRC_COMMON_H_

#include <QtCore/QList>
#include <QtCore/QString>

namespace Flix {

struct Connection
{
    Connection();

    QString name;
    QString host;
    int port;
    QString baseDn;
    QString authDn;
    QString authPassword;
    bool savePassword;
    QString subOu;
};

typedef QList<Connection> Connections;

typedef QList<QString> LdapDistinguishedNameParts;

QString joinDistinguishedName(const LdapDistinguishedNameParts& parts);

} /* namespace Flix */

#endif /* SRC_COMMON_H_ */
