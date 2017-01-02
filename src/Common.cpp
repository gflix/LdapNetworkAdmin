/*
 * Common.cpp
 *
 *  Created on: 01.01.2017
 *      Author: felix
 */

#include <Common.h>

namespace Flix {

Connection::Connection(void):
    port(0),
    savePassword(false)
{
}

QString joinDistinguishedName(const LdapDistinguishedNameParts& parts)
{
    QString distinguishedName;
    int index = 0;
    for (auto part: parts) {
        if (part.isEmpty()) {
            continue;
        }
        if (index > 0) {
            distinguishedName = ',' + distinguishedName;
        }
        distinguishedName = part + distinguishedName;
        ++index;
    }
    return distinguishedName;
}

} /* namespace Flix */
