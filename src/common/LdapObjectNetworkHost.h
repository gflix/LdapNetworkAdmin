/*
 * LdapObjectNetworkHost.h
 *
 *  Created on: 04.01.2017
 *      Author: felix
 */

#ifndef SRC_LDAPOBJECTNETWORKHOST_H_
#define SRC_LDAPOBJECTNETWORKHOST_H_

#include <common/GenericLdapObject.h>

namespace Flix {

class LdapObjectNetworkHost: public GenericLdapObject {
public:
    LdapObjectNetworkHost();
    virtual ~LdapObjectNetworkHost();

    virtual bool isValid(void) const;
    QString getIpAddress(void) const;
    QString getDescription(void) const;
    QString getMacAddress(void) const;

    void setIpAddress(const QString& ipAddress);
    void setDescription(const QString& ipAddress);
    void setMacAddress(const QString& macAddress);

    static LdapObjectNetworkHost* create(const QString& distinguishedName, const QString& ipAddress);
};

} /* namespace Flix */

#endif /* SRC_LDAPOBJECTNETWORKHOST_H_ */
