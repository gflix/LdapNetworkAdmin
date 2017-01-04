/*
 * LdapObjectOrganizationalUnit.h
 *
 *  Created on: 04.01.2017
 *      Author: felix
 */

#ifndef SRC_LDAPOBJECTORGANIZATIONALUNIT_H_
#define SRC_LDAPOBJECTORGANIZATIONALUNIT_H_

#include <GenericLdapObject.h>

namespace Flix {

class LdapObjectOrganizationalUnit: public GenericLdapObject {
public:
    LdapObjectOrganizationalUnit();
    virtual ~LdapObjectOrganizationalUnit();

    virtual bool isValid(void) const;

    static LdapObjectOrganizationalUnit* create(const QString& distinguishedName);
};

} /* namespace Flix */

#endif /* SRC_LDAPOBJECTORGANIZATIONALUNIT_H_ */
