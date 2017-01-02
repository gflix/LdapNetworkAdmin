/*
 * LdapObject.h
 *
 *  Created on: 02.01.2017
 *      Author: felix
 */

#ifndef SRC_LDAPOBJECT_H_
#define SRC_LDAPOBJECT_H_

#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>

namespace Flix {

typedef QList<QString> LdapAttributeValues;

class LdapObject {
    QString distinguishedName;
    QMap<QString, LdapAttributeValues> attributes;
public:
    LdapObject();
    virtual ~LdapObject();

    void clear(void);
    void setDistinguishedName(const QString& distinguishedName);
    void setAttribute(const QString& attribute, const LdapAttributeValues& values);

    bool isValid(void) const;
    const QString& getDistinguishedName(void) const;
    QString getShortName(void) const;
    bool hasAttribute(const QString& attribute) const;
    LdapAttributeValues getAttribute(const QString& attribute) const;

    static LdapObject createOrganizationalUnit(const QString& distinguishedName);
};

typedef QList<LdapObject> LdapObjects;

} /* namespace Flix */

#endif /* SRC_LDAPOBJECT_H_ */
