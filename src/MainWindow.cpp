/*
 * MainWindow.cpp
 *
 *  Created on: 31.12.2016
 *      Author: felix
 */

#include <QtCore/QDebug>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMessageBox>
#include <MainWindow.h>
#include <DialogConnections.h>
#include <DialogPassword.h>

namespace Flix {

MainWindow::MainWindow()
{
    setWindowTitleWithState();
    initActions();
    initMenuBar();
}

MainWindow::~MainWindow()
{
}

void MainWindow::showConnectionsDialog(void)
{
    DialogConnections dialog { this };
    if (dialog.exec() == QDialog::DialogCode::Accepted) {
        Connection connection;
        if (dialog.getConnection(connection)) {
            connectToLdapServer(connection);
        }
    }
}

void MainWindow::disconnectFromLdapServer(void)
{
    ldapConnection.unbind();
    setWindowTitleWithState();
    actionDisconnect->setEnabled(false);
}

void MainWindow::initActions(void)
{
    actionConnect = new QAction(tr("&Connect..."), this);
    actionConnect->setShortcut(Qt::CTRL | Qt::Key_C);
    connect(actionConnect, SIGNAL(triggered()), this, SLOT(showConnectionsDialog()));

    actionDisconnect = new QAction(tr("&Disconnect"), this);
    actionDisconnect->setShortcut(Qt::CTRL | Qt::Key_D);
    actionDisconnect->setEnabled(false);
    connect(actionDisconnect, SIGNAL(triggered()), this, SLOT(disconnectFromLdapServer()));

    actionQuit = new QAction(tr("&Quit"), this);
    actionQuit->setShortcut(Qt::CTRL | Qt::Key_Q);
    connect(actionQuit, SIGNAL(triggered()), this, SLOT(close()));
}

void MainWindow::initMenuBar(void)
{
    QMenu* menuEntry = menuBar()->addMenu(tr("LdapNetworkAdmin"));
    menuEntry->addAction(actionConnect);
    menuEntry->addAction(actionDisconnect);
    menuEntry->addSeparator();
    menuEntry->addAction(actionQuit);
}

void MainWindow::setWindowTitleWithState(const QString& state)
{
    QString baseTitle = tr("LDAP network admin");
    if (state.isEmpty()) {
        setWindowTitle(baseTitle);
    } else {
        setWindowTitle(QString("%1 (%2)").arg(baseTitle).arg(state));
    }
}

void MainWindow::connectToLdapServer(const Connection& connection)
{
    if (connection.host.isEmpty() || connection.baseDn.isEmpty() || connection.authDn.isEmpty()) {
        qWarning() << "WARNING: Insufficient connection details given. Aborting connection request.";
        return;
    }
    QString authPassword;
    if (connection.savePassword) {
        authPassword = connection.authPassword;
    } else {
        DialogPassword dialog { this };
        if (dialog.exec() != QDialog::DialogCode::Accepted) {
            return;
        }
        authPassword = dialog.getPassword();
    }
    if (ldapConnection.bind(connection.host.toStdString(), connection.port, connection.authDn.toStdString(), authPassword.toStdString())) {
        setWindowTitleWithState(connection.name);
        actionDisconnect->setEnabled(true);
    } else {
        disconnectFromLdapServer();
        QMessageBox::critical(this, tr("LDAP server connection"), tr("Could not connect to LDAP server!"));
    }
}

} /* namespace Flix */
