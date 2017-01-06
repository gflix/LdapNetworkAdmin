/*
 * LdapNetworkAdmin.cpp
 *
 *  Created on: 31.12.2016
 *      Author: felix
 */

#include <QtCore/QTranslator>
#include <QtWidgets/QApplication>
#include <frontend/MainWindow.h>

int main(int argc, char* argv[])
{
    QApplication application(argc, argv);
    QTranslator translator;
    translator.load(QLocale(), "ldap-network-admin", "_");
    application.installTranslator(&translator);

    Flix::MainWindow mainWindow;

    mainWindow.show();

    return application.exec();
}
