/*
 * LdapObjectNetworkHost.h
 *
 *  Created on: 04.01.2017
 *      Author: felix
 */

#ifndef SRC_LDAPOBJECTNETWORKHOST_H_
#define SRC_LDAPOBJECTNETWORKHOST_H_

#include <GenericLdapObject.h>

namespace Flix {

class LdapObjectNetworkHost: public GenericLdapObject {
public:
    LdapObjectNetworkHost();
    virtual ~LdapObjectNetworkHost();

    virtual bool isValid(void) const;

    static LdapObjectNetworkHost* create(const QString& distinguishedName, const QString& ipAddress);
};

} /* namespace Flix */

#endif /* SRC_LDAPOBJECTNETWORKHOST_H_ */
