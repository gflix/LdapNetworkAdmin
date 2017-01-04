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

    std::unique_ptr<char> distinguishedName { ldap_strdup(object.getDistinguishedName().toStdString().c_str()) };
    LDAPMod** ldapAttributes = generateLdapAttributes(object, LdapAttributeOperation::ADD);

    bool returnValue = ldap_add_ext_s(handle, distinguishedName.get(), ldapAttributes, nullptr, nullptr) == LDAP_SUCCESS;

    freeLdapAttributes(ldapAttributes);

    return returnValue;
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

bool LdapConnection::deleteObject(const LdapObject& object) const
{
    if (!bound && !object.isValid()) {
        return false;
    }

    std::unique_ptr<char> distinguishedName { ldap_strdup(object.getDistinguishedName().toStdString().c_str()) };

    return ldap_delete_ext_s(handle, distinguishedName.get(), nullptr, nullptr) == LDAP_SUCCESS;
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

LDAPMod** LdapConnection::generateLdapAttributes(const LdapObject& object, LdapAttributeOperation operation) const
{
    int ldapAttributeOperation;

    switch (operation) {
    case LdapAttributeOperation::MODIFY:
        ldapAttributeOperation = LDAP_MOD_REPLACE;
        break;
    default:
        ldapAttributeOperation = LDAP_MOD_ADD;
        break;
    }

    LdapAttributes attributes = object.getAttributes();
    size_t ldapAttributesSize = (attributes.size() + 1) * sizeof(LDAPMod*);
    LDAPMod** ldapAttributes = (LDAPMod**) ldap_memalloc(ldapAttributesSize);

    bzero(ldapAttributes, ldapAttributesSize);
    LDAPMod** ldapAttributesPointer = ldapAttributes;

    for (auto& attribute: attributes) {
        LDAPMod* ldapAttribute = (LDAPMod*) ldap_memalloc(sizeof(LDAPMod));
        ldapAttribute->mod_op = ldapAttributeOperation;
        ldapAttribute->mod_type = ldap_strdup(attribute.toStdString().c_str());

        LdapAttributeValues attributeValues = object.getAttribute(attribute);
        size_t ldapAttributeValuesSize = (attributeValues.size() + 1) * sizeof(char*);
        ldapAttribute->mod_values = (char**) ldap_memalloc(ldapAttributeValuesSize);

        bzero(ldapAttribute->mod_values, ldapAttributeValuesSize);
        char** ldapAttributesValuesPointer = ldapAttribute->mod_values;

        for (auto& attributeValue: attributeValues) {
            *ldapAttributesValuesPointer = ldap_strdup(attributeValue.toStdString().c_str());
            ++ldapAttributesValuesPointer;
        }

        *ldapAttributesPointer = ldapAttribute;
        ++ldapAttributesPointer;
    }

    return ldapAttributes;
}

void LdapConnection::freeLdapAttributes(LDAPMod** attributes) const
{
    if (!attributes) {
        return;
    }
    LDAPMod** ldapAttributesPointer = attributes;
    while (*ldapAttributesPointer) {
        if ((*ldapAttributesPointer)->mod_type) ldap_memfree((*ldapAttributesPointer)->mod_type);
        if ((*ldapAttributesPointer)->mod_values) ldap_memvfree((void**) (*ldapAttributesPointer)->mod_values);
        ++ldapAttributesPointer;
    }
    ldap_memvfree((void**) attributes);
}

} /* namespace Flix */
