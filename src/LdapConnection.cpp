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

} /* namespace Flix */
