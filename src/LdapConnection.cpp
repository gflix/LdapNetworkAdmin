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

bool LdapConnection::bind(const std::string& host, int port, const std::string& authDn, const std::string& authPassword)
{
    unbind();
    if (host.empty() || port < 1 || port > 65535) {
        return false;
    }
    std::string ldapUri = "ldap://" + host + ":" + std::to_string(port);
    if (ldap_initialize(&handle, ldapUri.c_str()) != LDAP_SUCCESS) {
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
    std::unique_ptr<char> authPasswordCopy { strdup(authPassword.c_str()) };
    authCredentials.bv_val = authPasswordCopy.get();
    authCredentials.bv_len = strlen(authPasswordCopy.get());
    if (ldap_sasl_bind_s(handle, authDn.c_str(), nullptr, &authCredentials, nullptr, nullptr, &serverCredentials) != LDAP_SUCCESS) {
        unbind();
        return false;
    }

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

} /* namespace Flix */
