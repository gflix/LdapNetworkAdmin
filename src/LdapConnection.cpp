/*
 * LdapConnection.cpp
 *
 *  Created on: 01.01.2017
 *      Author: felix
 */

#include <memory>
#include <QtCore/QDebug>
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

bool LdapConnection::addObject(const LdapObject& object) const
{
    if (!bound && !object.isValid()) {
        return false;
    }

    LDAPMod attributeObjectClass;
    LDAPMod* attributes[] = { &attributeObjectClass, nullptr };

    attributeObjectClass.mod_op = 0;
    attributeObjectClass.mod_type = strdup("objectClass");
    char* objectClassValues[] = { strdup("top"), strdup("organizationalUnit"), nullptr };
    attributeObjectClass.mod_values = objectClassValues;

    std::unique_ptr<char> distinguishedName { strdup(object.getDistinguishedName().toStdString().c_str()) };

    return ldap_add_ext_s(handle, distinguishedName.get(), attributes, nullptr, nullptr) == LDAP_SUCCESS;
}

bool LdapConnection::searchObjects(LdapObjects& objects, const QString& searchBaseDn, LdapSearchScope searchScope, const QString& filter) const
{
    objects.clear();
    if (!bound) {
        return false;
    }

    bool returnValue = true;
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
    LDAPMessage* ldapMessages = nullptr;
    LDAPMessage* ldapMessage;
    if (ldap_search_ext_s(handle, ldapSearchBaseDn.get(), ldapSearchScope, ldapFilter.get(), nullptr, 0, nullptr, nullptr, &timeout, LDAP_NO_LIMIT, &ldapMessages) != LDAP_SUCCESS) {
        return false;
    }
    Q_ASSERT(ldapMessages);

    LdapObject object;
    for (ldapMessage = ldap_first_message(handle, ldapMessages); ldapMessage; ldapMessage = ldap_next_message(handle, ldapMessage)) {
        int ldapMessageType = ldap_msgtype(ldapMessage);
        switch (ldapMessageType) {
        case LDAP_RES_SEARCH_ENTRY:
            retrieveLdapObject(ldapMessage, object);
            if (object.isValid()) {
                objects.push_back(object);
            } else {
                returnValue = false;
            }
            break;
        case LDAP_RES_SEARCH_RESULT:
            returnValue = ldap_parse_result(handle, ldapMessage, nullptr, nullptr, nullptr, nullptr, nullptr, 0) == LDAP_SUCCESS ? returnValue : false;
            break;
        default:
            break;
        }
    }

    ldap_msgfree(ldapMessages);

    return returnValue;
}

void LdapConnection::retrieveLdapObject(LDAPMessage* message, LdapObject& object) const
{
    object.clear();
    Q_ASSERT(bound);
    Q_ASSERT(ldap_msgtype(message) == LDAP_RES_SEARCH_ENTRY);

    char* ldapObjectDn = ldap_get_dn(handle, message);
    object.setDistinguishedName(ldapObjectDn);
    ldap_memfree(ldapObjectDn);

    char* ldapAttribute;
    berval** ldapAttributeValues;
    BerElement* binaryEncoding = nullptr;

    for (ldapAttribute = ldap_first_attribute(handle, message, &binaryEncoding); ldapAttribute; ldapAttribute = ldap_next_attribute(handle, message, binaryEncoding)) {
        ldapAttributeValues = ldap_get_values_len(handle, message, ldapAttribute);
        if (ldapAttributeValues) {
            LdapAttributeValues values;
            berval** ldapAttributeValuePointer = ldapAttributeValues;

            for (; ldapAttributeValuePointer && *ldapAttributeValuePointer; ++ldapAttributeValuePointer) {
                Q_ASSERT((*ldapAttributeValuePointer)->bv_val);
                values.push_back((*ldapAttributeValuePointer)->bv_val);
            }
            ldap_value_free_len(ldapAttributeValues);
            object.setAttribute(ldapAttribute, values);
        }

        ldap_memfree(ldapAttribute);
    }
}

} /* namespace Flix */
