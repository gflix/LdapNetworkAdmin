/*
 * LdapConnection.h
 *
 *  Created on: 01.01.2017
 *      Author: felix
 */

#ifndef SRC_LDAPCONNECTION_H_
#define SRC_LDAPCONNECTION_H_

#include <ldap.h>
#include <Common.h>

namespace Flix {

class LdapConnection {
    LDAP* handle;

    bool bound;
    Connection connection;
public:
    LdapConnection();
    virtual ~LdapConnection();

    bool bind(const Connection& connection, const QString& authPassword);
    void unbind();

    bool isBound(void) const;
    const Connection& getConnection(void) const;
};

} /* namespace Flix */

#endif /* SRC_LDAPCONNECTION_H_ */
