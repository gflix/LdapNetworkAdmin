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
#include <GenericLdapObject.h>

namespace Flix {

enum class LdapSearchScope {
    BASE,
    ONE,
    SUBTREE
};

enum class LdapAttributeOperation {
    ADD,
    MODIFY,
};

class LdapConnection {
    LDAP* handle;

    bool bound;
    Connection connection;

    void retrieveLdapObject(LDAPMessage* message, GenericLdapObject** object) const;
    LDAPMod** generateLdapAttributes(const GenericLdapObject* object, LdapAttributeOperation operation) const;
    void freeLdapAttributes(LDAPMod** attributes) const;
public:
    LdapConnection();
    virtual ~LdapConnection();

    bool bind(const Connection& connection, const QString& authPassword);
    void unbind();

    bool isBound(void) const;
    const Connection& getConnection(void) const;

    bool addObject(const GenericLdapObject* object) const;
    bool searchObjects(LdapObjects& objects, const QString& searchBaseDn, LdapSearchScope searchScope = LdapSearchScope::BASE, const QString& filter = QString("(objectClass=*)")) const;
    bool deleteObject(const GenericLdapObject* object) const;
    bool renameObject(GenericLdapObject** object, const QString& newIdentifier) const;
};

} /* namespace Flix */

#endif /* SRC_LDAPCONNECTION_H_ */
