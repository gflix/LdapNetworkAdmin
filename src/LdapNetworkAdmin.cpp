/*
 * LdapNetworkAdmin.cpp
 *
 *  Created on: 31.12.2016
 *      Author: felix
 */

#include <QtWidgets/QApplication>
#include <MainWindow.h>

int main(int argc, char* argv[])
{
    QApplication application(argc, argv);
    Flix::MainWindow mainWindow;

    mainWindow.show();

    return application.exec();
}
