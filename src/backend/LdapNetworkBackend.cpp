/*
 * LdapNetworkBackend.cpp
 *
 *  Created on: 06.01.2017
 *      Author: felix
 */

#include <iostream>
#include <QtCore/QList>
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
    cout << "  host       = " << object->getIdentifier().toStdString() << endl;
    cout << "  ipAddress  = " << object->getIpAddress().toStdString() << endl;
    cout << "  macAddress = " << object->getMacAddress().toStdString() << endl;

    QRegExp regexHostParts { R"re(^([\w-]+)\.(.+)$)re" };
    if (!regexHostParts.exactMatch(object->getIdentifier())) {
        return;
    }

    QRegExp regexIpAddressParts { R"re(^(\d{1,3})\.(\d{1,3})\.(\d{1,3})\.(\d{1,3})$)re" };
    if (!regexIpAddressParts.exactMatch(object->getIpAddress())) {
        return;
    }

    Flix::DnsZoneEntryA* dnsZoneEntryA = new Flix::DnsZoneEntryA(regexHostParts.cap(1), object->getIpAddress());
    dnsForwardZones[regexHostParts.cap(2)].push_back(Flix::DnsZoneEntry(dnsZoneEntryA));

    Flix::DnsZoneEntryPtr* dnsZoneEntryPtr = new Flix::DnsZoneEntryPtr(object->getIdentifier(), regexIpAddressParts.cap(4));
    QString dnsReverseZone {
        regexIpAddressParts.cap(3) + '.' +
        regexIpAddressParts.cap(2) + '.' +
        regexIpAddressParts.cap(1) + ".in-addr.arpa"};
    dnsReverseZones[dnsReverseZone].push_back(Flix::DnsZoneEntry(dnsZoneEntryPtr));

    if (!object->getMacAddress().isEmpty()) {
        Flix::DhcpHost dhcpHost;
        dhcpHost.fullyQualifiedHostname = object->getIdentifier();
        dhcpHost.macAddress = object->getMacAddress();
        dhcpHosts.push_back(dhcpHost);
    }
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
        cerr << "ERROR: Could not load configuration! Aborting." << endl;
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

    cout << "== LDAP ==" << endl;
    for (auto& object: objects) {
        Flix::GenericLdapObject* ldapObject = object.get();
        if (!ldapObject || !ldapObject->isObjectType(Flix::LdapObjectType::NETWORK_HOST)) {
            continue;
        }
        cout << ldapObject->getDistinguishedName().toStdString() << endl;
        processNetworkHost((Flix::LdapObjectNetworkHost*) ldapObject, dhcpHosts, dnsForwardZones, dnsReverseZones);
    }

    cout << endl;
    cout << "== DHCP ==" << endl;
    for (auto& dhcpHost: dhcpHosts) {
        cout << dhcpHost.fullyQualifiedHostname.toStdString() << ": " << dhcpHost.macAddress.toStdString() << endl;
    }

    cout << endl;
    cout << "== DNS (forward) ==" << endl;
    for (auto& dnsForwardZone: dnsForwardZones.keys()) {
        cout << "zone = " << dnsForwardZone.toStdString() << ", entries = " << dnsForwardZones[dnsForwardZone].size() << endl;
        for (auto& dnsFowardEntry: dnsForwardZones[dnsForwardZone]) {
            cout << "  " << dnsFowardEntry.get()->getRecord().toStdString() << endl;
        }
    }

    cout << endl;
    cout << "== DNS (reverse) ==" << endl;
    for (auto& dnsReverseZone: dnsReverseZones.keys()) {
        cout << "zone = " << dnsReverseZone.toStdString() << ", entries = " << dnsReverseZones[dnsReverseZone].size() << endl;
        for (auto& dnsReverseEntry: dnsReverseZones[dnsReverseZone]) {
            cout << "  " << dnsReverseEntry.get()->getRecord().toStdString() << endl;
        }
    }

    return 0;
}
