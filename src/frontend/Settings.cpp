/*
 * Settings.cpp
 *
 *  Created on: 09.01.2017
 *      Author: felix
 */

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QStandardPaths>
#include <QtCore/QTextStream>
#include <QtXml/QDomDocument>
#include <frontend/Settings.h>

#define FILENAME_SETTINGS "LdapNetworkAdminSettings.xml"

#define XML_ELEMENT_SETTINGS "Settings"
#define XML_ELEMENT_DEFAULTS "Defaults"

#define XML_ATTRIBUTE_DOMAIN_SUFFIX "domainSuffix"
#define XML_ATTRIBUTE_IP_ADDRESS_PREFIX "ipAddressPrefix"

namespace Flix {

Settings::Settings()
{
    load();
}

Settings::~Settings()
{
}

void Settings::load(void)
{
    QFile fileSettings(QStandardPaths::locate(QStandardPaths::ConfigLocation, FILENAME_SETTINGS));
    if (!fileSettings.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QDomDocument domSettings;
    if (!domSettings.setContent(&fileSettings)) {
        return;
    }
    QDomElement elementSettings = domSettings.documentElement();
    if (elementSettings.tagName() != XML_ELEMENT_SETTINGS) {
        return;
    }

    QDomElement elementDefaults = elementSettings.firstChildElement(XML_ELEMENT_DEFAULTS);
    if (!elementDefaults.isNull()) {
        defaults.domainSuffix = elementDefaults.attribute(XML_ATTRIBUTE_DOMAIN_SUFFIX);
        defaults.ipAddressPrefix = elementDefaults.attribute(XML_ATTRIBUTE_IP_ADDRESS_PREFIX);
    }
}

bool Settings::save(void) const
{
    QDir dirSettings { QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) };
    QFile fileSettings(dirSettings.filePath(FILENAME_SETTINGS));

    if (!fileSettings.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QDomDocument domSettings;
    QDomProcessingInstruction processingInstruction =
        domSettings.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\"");
    domSettings.appendChild(processingInstruction);

    QDomElement elementSettings = domSettings.createElement(XML_ELEMENT_SETTINGS);

    QDomElement elementDefaults = domSettings.createElement(XML_ELEMENT_DEFAULTS);
    elementDefaults.setAttribute(XML_ATTRIBUTE_DOMAIN_SUFFIX, defaults.domainSuffix);
    elementDefaults.setAttribute(XML_ATTRIBUTE_IP_ADDRESS_PREFIX, defaults.ipAddressPrefix);
    elementSettings.appendChild(elementDefaults);

    domSettings.appendChild(elementSettings);

    QTextStream streamSettings(&fileSettings);
    domSettings.save(streamSettings, 2);

    return true;
}

const SettingsDefaults& Settings::getDefaults(void) const
{
    return defaults;
}

void Settings::setDefaults(const SettingsDefaults& defaults)
{
    this->defaults = defaults;
}

} /* namespace Flix */
