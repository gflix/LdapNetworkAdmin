/*
 * Configuration.h
 *
 *  Created on: 07.01.2017
 *      Author: felix
 */

#ifndef SRC_BACKEND_CONFIGURATIONLOADER_H_
#define SRC_BACKEND_CONFIGURATIONLOADER_H_

#include <QtCore/QString>

namespace Flix {

struct LdapNetworkBackendConnection {
    QString host;
    int port;
    QString baseDn;
};

struct LdapNetworkBackendConfiguration {
    LdapNetworkBackendConnection connection;
};

class ConfigurationLoader {
public:
    ConfigurationLoader();
    virtual ~ConfigurationLoader();

    void load(const QString& filename);

    bool isValid(void) const;
    const LdapNetworkBackendConfiguration& getConfiguration(void) const;

private:
    bool valid;
    LdapNetworkBackendConfiguration configuration;
};

} /* namespace Flix */

#endif /* SRC_BACKEND_CONFIGURATIONLOADER_H_ */
