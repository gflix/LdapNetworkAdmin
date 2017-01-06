/*
 * LdapObjectDcObject.h
 *
 *  Created on: 04.01.2017
 *      Author: felix
 */

#ifndef SRC_LDAPOBJECTDCOBJECT_H_
#define SRC_LDAPOBJECTDCOBJECT_H_

#include <common/GenericLdapObject.h>

namespace Flix {

class LdapObjectDcObject: public GenericLdapObject {
public:
    LdapObjectDcObject();
    virtual ~LdapObjectDcObject();

    virtual bool isValid(void) const;
};

} /* namespace Flix */

#endif /* SRC_LDAPOBJECTDCOBJECT_H_ */
