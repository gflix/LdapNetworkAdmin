/*
 * LdapObject.h
 *
 *  Created on: 02.01.2017
 *      Author: felix
 */

#ifndef SRC_GENERICLDAPOBJECT_H_
#define SRC_GENERICLDAPOBJECT_H_

#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>

namespace Flix {

typedef QList<QString> LdapAttributes;
typedef QList<QString> LdapAttributeValues;

enum class LdapObjectType {
    UNKNOWN,
    DC_OBJECT,
    ORGANIZATIONAL_UNIT,
    NETWORK_HOST,
};

class GenericLdapObject {
public:
    GenericLdapObject(LdapObjectType type);
    virtual ~GenericLdapObject();

    void clear(void);
    void setDistinguishedName(const QString& distinguishedName);
    void setAttribute(const QString& attribute, const LdapAttributeValues& values);

    virtual bool isValid(void) const = 0;
    LdapObjectType getType(void) const;
    bool isObjectType(LdapObjectType type) const;
    const QString& getDistinguishedName(void) const;
    QString getIdentifier(void) const;
    LdapAttributes getAttributes(void) const;
    bool hasAttribute(const QString& attribute) const;
    LdapAttributeValues getAttribute(const QString& attribute) const;

    static GenericLdapObject* getDuplicate(const GenericLdapObject* original);

//    bool isDcObject(void) const;
//    bool isOrganizationalUnit(void) const;
//    bool isNetworkHost(void) const;

//    static GenericLdapObject createOrganizationalUnit(const QString& distinguishedName);
//    static GenericLdapObject createNetworkHost(const QString& distinguishedName);
protected:
    QString distinguishedName;

private:
    LdapObjectType type;
    QMap<QString, LdapAttributeValues> attributes;
};

typedef QList<const GenericLdapObject*> LdapObjects;

} /* namespace Flix */

#endif /* SRC_GENERICLDAPOBJECT_H_ */
