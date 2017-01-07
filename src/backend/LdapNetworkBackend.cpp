/*
 * LdapNetworkBackend.cpp
 *
 *  Created on: 06.01.2017
 *      Author: felix
 */

#include <iostream>
#include <QtCore/QList>
#include <backend/ConfigurationLoader.h>
#include <common/LdapConnection.h>
#include <common/LdapObjectNetworkHost.h>

using namespace std;

struct DhcpHost {
    QString host;
    QString macAddress;
};

typedef QList<DhcpHost> DhcpHosts;

void printUsage(const QString& programName)
{
    cout << "Usage: " << programName.toStdString() << " CONFIG" << endl;
    cout << endl;
    cout << "Arguments:" << endl;
    cout << "  CONFIG  XML configuration file" << endl;
    cout << endl;
}

void processNetworkHost(const Flix::LdapObjectNetworkHost* object, DhcpHosts& dhcpHosts)
{
    if (!object) {
        return;
    }
    cout << "  host       = " << object->getIdentifier().toStdString() << endl;
    cout << "  ipAddress  = " << object->getIpAddress().toStdString() << endl;
    cout << "  macAddress = " << object->getMacAddress().toStdString() << endl;

    if (!object->getMacAddress().isEmpty()) {
        DhcpHost dhcpHost;
        dhcpHost.host = object->getIdentifier();
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

    DhcpHosts dhcpHosts;

    cout << "== LDAP ==" << endl;
    for (auto& object: objects) {
        Flix::GenericLdapObject* ldapObject = object.get();
        if (!ldapObject || !ldapObject->isObjectType(Flix::LdapObjectType::NETWORK_HOST)) {
            continue;
        }
        cout << ldapObject->getDistinguishedName().toStdString() << endl;
        processNetworkHost((Flix::LdapObjectNetworkHost*) ldapObject, dhcpHosts);
    }

    cout << endl;
    cout << "== DHCP ==" << endl;
    for (auto& dhcpHost: dhcpHosts) {
        cout << dhcpHost.host.toStdString() << ": " << dhcpHost.macAddress.toStdString() << endl;
    }

    return 0;
}
