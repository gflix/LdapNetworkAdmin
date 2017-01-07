/*
 * Configuration.cpp
 *
 *  Created on: 07.01.2017
 *      Author: felix
 */

#include <backend/ConfigurationLoader.h>

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
    // TODO: needs to be implemented
}

bool ConfigurationLoader::isValid(void) const
{
    return valid;
}

const LdapNetworkBackendConfiguration& ConfigurationLoader::getConfiguration(void) const
{
    return configuration;
}

} /* namespace Flix */
