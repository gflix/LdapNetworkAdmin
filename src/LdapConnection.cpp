/*
 * LdapConnection.cpp
 *
 *  Created on: 01.01.2017
 *      Author: felix
 */

#include <memory>
#include <LdapConnection.h>

namespace Flix {

LdapConnection::LdapConnection():
    handle(nullptr),
    bound(false)
{
}

LdapConnection::~LdapConnection()
{
    unbind();
}

bool LdapConnection::bind(const Connection& newConnection, const QString& authPassword)
{
    unbind();

    if (newConnection.host.isEmpty() || newConnection.port < 1 || newConnection.port > 65535) {
        return false;
    }

    QString ldapUri= QString("ldap://%1:%2/").arg(newConnection.host).arg(newConnection.port);
    if (ldap_initialize(&handle, ldapUri.toStdString().c_str()) != LDAP_SUCCESS) {
        unbind();
        return false;
    }

    int version = LDAP_VERSION3;
    if (ldap_set_option(handle, LDAP_OPT_PROTOCOL_VERSION, &version) != LDAP_OPT_SUCCESS) {
        unbind();
        return false;
    }

    berval authCredentials;
    berval* serverCredentials = nullptr;
    std::unique_ptr<char> authPasswordCopy { strdup(authPassword.toStdString().c_str()) };
    authCredentials.bv_val = authPasswordCopy.get();
    authCredentials.bv_len = strlen(authPasswordCopy.get());
    if (ldap_sasl_bind_s(handle, newConnection.authDn.toStdString().c_str(), nullptr, &authCredentials, nullptr, nullptr, &serverCredentials) != LDAP_SUCCESS) {
        unbind();
        return false;
    }

    connection = newConnection;
    bound = true;
    return true;
}

void LdapConnection::unbind()
{
    if (handle) {
        if (bound) {
            ldap_unbind_ext_s(handle, nullptr, nullptr);
        } else {
            ldap_destroy(handle);
        }
    }
    handle = nullptr;
    bound = false;
}

bool LdapConnection::isBound(void) const
{
    return bound;
}

const Connection& LdapConnection::getConnection(void) const
{
    return connection;
}

bool LdapConnection::search(LdapSearchScope searchScope, const QString& filter)
{
    if (!bound) {
        return false;
    }

    QString searchBaseDn;
    if (!connection.subOu.isEmpty()) {
        searchBaseDn = connection.subOu + ",";
    }
    searchBaseDn += connection.baseDn;
    std::unique_ptr<char> ldapSearchBaseDn { strdup(searchBaseDn.toStdString().c_str()) };
    int ldapSearchScope;
    switch (searchScope) {
    case LdapSearchScope::ONE:
        ldapSearchScope = LDAP_SCOPE_ONE;
        break;
    case LdapSearchScope::SUBTREE:
        ldapSearchScope = LDAP_SCOPE_SUBTREE;
        break;
    default:
        ldapSearchScope = LDAP_SCOPE_BASE;
        break;
    }
    std::unique_ptr<char> ldapFilter { strdup(filter.toStdString().c_str()) };
    timeval timeout;
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;
    LDAPMessage* ldapMessage = nullptr;
    if (ldap_search_ext_s(handle, ldapSearchBaseDn.get(), ldapSearchScope, ldapFilter.get(), nullptr, 0, nullptr, nullptr, &timeout, 256, &ldapMessage) != LDAP_SUCCESS) {
        return false;
    }
    Q_ASSERT(ldapMessage);
    ldap_msgfree(ldapMessage);

    return true;
}

} /* namespace Flix */
