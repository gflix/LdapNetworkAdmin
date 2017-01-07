/*
 * Configuration.cpp
 *
 *  Created on: 07.01.2017
 *      Author: felix
 */

#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <backend/ConfigurationLoader.h>

#define XML_ELEMENT_CONNECTION "Connection"
#define XML_ELEMENT_LDAP_ADMIN_BACKEND "LdapAdminBackend"

#define XML_ATTRIBUTE_BASE_DN "baseDn"
#define XML_ATTRIBUTE_HOST "host"
#define XML_ATTRIBUTE_PORT "port"

namespace Flix {

ConfigurationLoader::ConfigurationLoader():
    valid(false)
{
}

ConfigurationLoader::~ConfigurationLoader()
{
}

void ConfigurationLoader::load(const QString& filename)
{
    valid = false;

    QFile fileConfig(filename);
    if (!fileConfig.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QDomDocument domConfig;
    if (!domConfig.setContent(&fileConfig)) {
        return;
    }
    QDomElement elementConfig = domConfig.documentElement();
    if (elementConfig.tagName() != XML_ELEMENT_LDAP_ADMIN_BACKEND) {
        return;
    }

    valid = load(elementConfig);
}

bool ConfigurationLoader::isValid(void) const
{
    return valid;
}

const LdapNetworkBackendConfiguration& ConfigurationLoader::getConfiguration(void) const
{
    return configuration;
}

bool ConfigurationLoader::load(QDomElement& elementConfig)
{
    QDomElement elementConnection = elementConfig.firstChildElement(XML_ELEMENT_CONNECTION);
    if (elementConnection.isNull()) {
        return false;
    }
    configuration.connection.host = elementConnection.attribute(XML_ATTRIBUTE_HOST);
    configuration.connection.port = elementConnection.attribute(XML_ATTRIBUTE_PORT, "0").toInt(nullptr, 10);
    configuration.connection.baseDn = elementConnection.attribute(XML_ATTRIBUTE_BASE_DN);

    if (configuration.connection.host.isEmpty() ||
        configuration.connection.port < 1 ||
        configuration.connection.port > 65535 ||
        configuration.connection.baseDn.isEmpty()) {
        return false;
    }

    return true;
}


} /* namespace Flix */
