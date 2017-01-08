/*
 * Configuration.h
 *
 *  Created on: 07.01.2017
 *      Author: felix
 */

#ifndef SRC_BACKEND_CONFIGURATIONLOADER_H_
#define SRC_BACKEND_CONFIGURATIONLOADER_H_

#include <QtCore/QString>
#include <QtXml/QDomDocument>

namespace Flix {

struct LdapNetworkBackendConnection {
    QString host;
    int port;
    QString baseDn;
};

struct LdapNetworkBackendDns {
    QString authoritativeNameserver;
    int ttl;
};

struct LdapNetworkBackendConfiguration {
    QString outputPath;
    LdapNetworkBackendConnection connection;
    LdapNetworkBackendDns dns;
};

class ConfigurationLoader {
public:
    ConfigurationLoader();
    virtual ~ConfigurationLoader();

    void load(const QString& filename);

    bool isValid(void) const;
    const LdapNetworkBackendConfiguration& getConfiguration(void) const;
    const QString& getErrorMessage(void);

private:
    bool valid;
    QString errorMessage;
    LdapNetworkBackendConfiguration configuration;

    bool load(QDomElement& elementConfig);
};

} /* namespace Flix */

#endif /* SRC_BACKEND_CONFIGURATIONLOADER_H_ */
