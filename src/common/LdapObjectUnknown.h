/*
 * LdapObjectUnknown.h
 *
 *  Created on: 05.01.2017
 *      Author: felix
 */

#ifndef SRC_LDAPOBJECTUNKNOWN_H_
#define SRC_LDAPOBJECTUNKNOWN_H_

#include <common/GenericLdapObject.h>

namespace Flix {

class LdapObjectUnknown: public GenericLdapObject {
public:
    LdapObjectUnknown();
    virtual ~LdapObjectUnknown();

    virtual bool isValid(void) const;
};

} /* namespace Flix */

#endif /* SRC_LDAPOBJECTUNKNOWN_H_ */
