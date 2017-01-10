/*
 * LdapNetworkBackend.cpp
 *
 *  Created on: 06.01.2017
 *      Author: felix
 */

#include <iostream>
#include <QtCore/QDateTime>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QList>
#include <QtCore/QTextStream>
#include <backend/ConfigurationLoader.h>
#include <backend/DhcpHost.h>
#include <backend/DnsZoneEntryA.h>
#include <backend/DnsZoneEntryPtr.h>
#include <common/LdapConnection.h>
#include <common/LdapObjectNetworkHost.h>

using namespace std;

void printUsage(const QString& programName)
{
    cout << "Usage: " << programName.toStdString() << " CONFIG" << endl;
    cout << endl;
    cout << "Arguments:" << endl;
    cout << "  CONFIG  XML configuration file" << endl;
    cout << endl;
}

void processNetworkHost(const Flix::LdapObjectNetworkHost* object, Flix::DhcpHosts& dhcpHosts, Flix::DnsZones& dnsForwardZones, Flix::DnsZones& dnsReverseZones)
{
    if (!object) {
        return;
    }

    QRegExp regexHostParts { R"re(^([\w-]+)\.(.+)$)re" };
    if (!regexHostParts.exactMatch(object->getIdentifier())) {
        return;
    }

    QRegExp regexIpAddressParts { R"re(^(\d{1,3})\.(\d{1,3})\.(\d{1,3})\.(\d{1,3})$)re" };
    if (!regexIpAddressParts.exactMatch(object->getIpAddress())) {
        return;
    }

    Flix::DnsZoneEntryA* dnsZoneEntryA = new Flix::DnsZoneEntryA(object->getDescription(), regexHostParts.cap(1), object->getIpAddress());
    dnsForwardZones[regexHostParts.cap(2)].push_back(Flix::DnsZoneEntry(dnsZoneEntryA));

    Flix::DnsZoneEntryPtr* dnsZoneEntryPtr = new Flix::DnsZoneEntryPtr(object->getDescription(), object->getIdentifier(), regexIpAddressParts.cap(4));
    QString dnsReverseZone {
        regexIpAddressParts.cap(3) + '.' +
        regexIpAddressParts.cap(2) + '.' +
        regexIpAddressParts.cap(1) + ".in-addr.arpa"};
    dnsReverseZones[dnsReverseZone].push_back(Flix::DnsZoneEntry(dnsZoneEntryPtr));

    if (!object->getMacAddress().isEmpty()) {
        Flix::DhcpHost dhcpHost;
        dhcpHost.fullyQualifiedHostname = object->getIdentifier();
        dhcpHost.description = object->getDescription();
        dhcpHost.macAddress = object->getMacAddress();
        dhcpHosts.push_back(dhcpHost);
    }
}

bool generateDhcpHostsFile(const QDir& dirOutput, const Flix::DhcpHosts& dhcpHosts)
{
    QFile fileDhcpHosts { dirOutput.filePath("dhcp-hosts.conf") };
    if (!fileDhcpHosts.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }
    QTextStream streamDhcpHosts { &fileDhcpHosts };

    streamDhcpHosts << "# This file was created automatically" << endl;
    streamDhcpHosts << "# Do NOT edit!" << endl;
    streamDhcpHosts << endl;
    streamDhcpHosts << "group {" << endl;

    for (auto& dhcpHost: dhcpHosts) {
        streamDhcpHosts << "  # " << dhcpHost.description << endl;
        streamDhcpHosts << "  host " << dhcpHost.fullyQualifiedHostname << " {" << endl;
        streamDhcpHosts << "    hardware ethernet " << dhcpHost.macAddress << ";" << endl;
        streamDhcpHosts << "    fixed-address " << dhcpHost.fullyQualifiedHostname << ";" << endl;
        streamDhcpHosts << "  }" << endl;
        streamDhcpHosts << endl;
    }

    streamDhcpHosts << "}" << endl;

    return true;
}

bool generateDnsForwardZoneFile(const QDir& dirOutput, const QString& serialNumber, const Flix::LdapNetworkBackendDns& configurationDns, const QString& dnsForwardZoneIdentifier, const Flix::DnsZoneEntries& dnsForwardZoneEntries)
{
    QFile fileDnsZone { dirOutput.filePath("db." + dnsForwardZoneIdentifier) };
    if (!fileDnsZone.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }
    QTextStream streamDnsZone { &fileDnsZone };

    streamDnsZone << "; This file was created automatically" << endl;
    streamDnsZone << "; Do NOT edit!" << endl;
    streamDnsZone << endl;
    streamDnsZone << "$TTL " << configurationDns.ttl << endl;
    streamDnsZone << "$ORIGIN " << dnsForwardZoneIdentifier << "." << endl;
    streamDnsZone <<
        "@ IN SOA " << configurationDns.authoritativeNameserver <<
        ". root." << dnsForwardZoneIdentifier <<
        ". " << serialNumber << " 4H 12H 1w 3H" << endl;
    streamDnsZone << "@ IN NS " << configurationDns.authoritativeNameserver << "." << endl;
    if (!configurationDns.zoneDefaultIpAddress.isEmpty()) {
        streamDnsZone << "@ IN A " << configurationDns.zoneDefaultIpAddress << endl;
    }
    streamDnsZone << endl;

    for (auto& dnsZoneEntry: dnsForwardZoneEntries) {
        Flix::GenericDnsZoneEntry* entry = dnsZoneEntry.get();
        if (!entry) {
            continue;
        }
        streamDnsZone << "; " << entry->getDescription() << endl;
        streamDnsZone << entry->getRecord() << endl;
        streamDnsZone << endl;
    }

    return true;
}

bool generateDnsForwardZoneFiles(const QDir& dirOutput, const QString& serialNumber, const Flix::LdapNetworkBackendDns& configurationDns, const Flix::DnsZones& dnsForwardZones)
{
    for (auto& dnsForwardZone: dnsForwardZones) {
        if (!generateDnsForwardZoneFile(dirOutput, serialNumber, configurationDns, dnsForwardZones.key(dnsForwardZone), dnsForwardZone)) {
            return false;
        }
    }
    return true;
}

void writeDnsConfigurationEntry(QTextStream& stream, const QDir& dirOutput, const QString& dnsZone)
{
    stream << "zone \"" << dnsZone << "\" {" << endl;
    stream << "  type master;" << endl;
    stream << "  file \"" << dirOutput.absolutePath() << "/db." << dnsZone << "\";" << endl;
    stream << "};" << endl;
    stream << endl;
}

bool generateDnsConfigurationFile(const QDir& dirOutput, const Flix::DnsZones& dnsForwardZones, const Flix::DnsZones& dnsReverseZones)
{
    QFile fileDnsConfiguration { dirOutput.filePath("named-zones.conf") };
    if (!fileDnsConfiguration.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }
    QTextStream streamDnsConfiguration { &fileDnsConfiguration };

    streamDnsConfiguration << "// This file was created automatically" << endl;
    streamDnsConfiguration << "// Do NOT edit!" << endl;
    streamDnsConfiguration << endl;

    for (auto& dnsForwardZone: dnsForwardZones.keys()) {
        writeDnsConfigurationEntry(streamDnsConfiguration, dirOutput, dnsForwardZone);
    }

    for (auto& dnsReverseZone: dnsReverseZones.keys()) {
        writeDnsConfigurationEntry(streamDnsConfiguration, dirOutput, dnsReverseZone);
    }

    return true;
}

int main(int argc, char* argv[])
{
    QString programName { argv[0] };

    if (argc < 2) {
        printUsage(programName);
        return 1;
    }

    Flix::ConfigurationLoader configurationLoader;
    configurationLoader.load(argv[1]);

    if (!configurationLoader.isValid()) {
        cerr << "ERROR: Could not load configuration (" << configurationLoader.getErrorMessage().toStdString() << ")! Aborting." << endl;
        return 2;
    }

    const Flix::LdapNetworkBackendConfiguration& configuration = configurationLoader.getConfiguration();
    Flix::Connection connection;
    connection.host = configuration.connection.host;
    connection.port = configuration.connection.port;
    connection.baseDn = configuration.connection.baseDn;
    connection.anonymousBind = true;
    Flix::LdapConnection ldapConnection;
    if (!ldapConnection.bind(connection)) {
        cerr << "ERROR: Could not connect to LDAP server! Aborting." << endl;
        return 3;
    }

    Flix::LdapObjects objects;
    if (!ldapConnection.searchObjects(objects, connection.baseDn, Flix::LdapSearchScope::SUBTREE, "(&(objectClass=device)(objectClass=ipHost)(objectClass=ieee802Device))")) {
        cerr << "ERROR: Unable to enumerate objects at the given DN \"" << connection.baseDn.toStdString() << "\"! Aborting." << endl;
        return 4;
    }

    Flix::DhcpHosts dhcpHosts;
    Flix::DnsZones dnsForwardZones;
    Flix::DnsZones dnsReverseZones;

    for (auto& object: objects) {
        Flix::GenericLdapObject* ldapObject = object.get();
        if (!ldapObject || !ldapObject->isObjectType(Flix::LdapObjectType::NETWORK_HOST)) {
            continue;
        }
        processNetworkHost((Flix::LdapObjectNetworkHost*) ldapObject, dhcpHosts, dnsForwardZones, dnsReverseZones);
    }

    if (!QDir().mkpath(configuration.outputPath)) {
        cerr << "ERROR: Unable to create the output path! Aborting." << endl;
        return 5;
    }

    QDir dirOutput { configuration.outputPath };

    if (!generateDhcpHostsFile(dirOutput, dhcpHosts)) {
        cerr << "ERROR: Unable to create the DHCP hosts file! Aborting." << endl;
        return 5;
    }

    QString dnsSerialNumber { QDateTime::currentDateTime().toString("yyMMddHHmm") };
    if (!generateDnsForwardZoneFiles(dirOutput, dnsSerialNumber, configuration.dns, dnsForwardZones)) {
        cerr << "ERROR: Unable to create the DNS forward zone files! Aborting." << endl;
        return 5;
    }
    if (!generateDnsForwardZoneFiles(dirOutput, dnsSerialNumber, configuration.dns, dnsReverseZones)) {
        cerr << "ERROR: Unable to create the DNS reverse zone files! Aborting." << endl;
        return 5;
    }
    if (!generateDnsConfigurationFile(dirOutput, dnsForwardZones, dnsReverseZones)) {
        cerr << "ERROR: Unable to create the DNS configuration file! Aborting." << endl;
        return 5;
    }

    return 0;
}
