/*
 * LdapConnection.h
 *
 *  Created on: 01.01.2017
 *      Author: felix
 */

#ifndef SRC_LDAPCONNECTION_H_
#define SRC_LDAPCONNECTION_H_

#include <ldap.h>
#include <string>

namespace Flix {

class LdapConnection {
    LDAP* handle;

    bool bound;
public:
    LdapConnection();
    virtual ~LdapConnection();

    bool bind(const std::string& host, int port, const std::string& authDn, const std::string& authPassword);
    void unbind();

    bool isBound(void) const;
};

} /* namespace Flix */

#endif /* SRC_LDAPCONNECTION_H_ */
