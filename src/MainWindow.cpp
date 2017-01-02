/*
 * MainWindow.cpp
 *
 *  Created on: 31.12.2016
 *      Author: felix
 */

#include <QtCore/QDebug>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMessageBox>
#include <MainWindow.h>
#include <DialogConnections.h>
#include <DialogPassword.h>

namespace Flix {

MainWindow::MainWindow()
{
    setWindowTitleWithState();

    networkTree = new ModelNetworkTree(this);

    initActions();
    initMenuBar();
    initLayout();
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
            if (!connectToLdapServer(connection)) {
                disconnectFromLdapServer();
                QMessageBox::critical(this, tr("LDAP server connection"), tr("Could not connect to LDAP server!"));
            } else {
                setWindowTitleWithState(connection.name);
                actionRefresh->setEnabled(true);
                actionDisconnect->setEnabled(true);
                emit(connectedToLdapServer());
            }
        }
    }
}

void MainWindow::disconnectFromLdapServer(void)
{
    ldapConnection.unbind();
    setWindowTitleWithState();
    actionRefresh->setEnabled(false);
    actionDisconnect->setEnabled(false);
}

void MainWindow::updateNetworkTree(void)
{
    if (!ldapConnection.isBound()) {
        return;
    }
    qInfo() << "MainWindow::updateNetworkTree()";

    const Connection& connection = ldapConnection.getConnection();
    LdapObjects objects;
    if (ldapConnection.search(objects, joinDistinguishedName({connection.baseDn, connection.subOu}), LdapSearchScope::BASE) && objects.size() == 1) {
        qInfo() << "INFO: Search was successful, entries=" << objects.size();
    }
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

    actionRefresh = new QAction(tr("&Refresh"), this);
    actionRefresh->setShortcut(Qt::CTRL | Qt::Key_R);
    actionRefresh->setEnabled(false);
    connect(actionRefresh, SIGNAL(triggered()), this, SLOT(updateNetworkTree()));

    actionQuit = new QAction(tr("&Quit"), this);
    actionQuit->setShortcut(Qt::CTRL | Qt::Key_Q);
    connect(actionQuit, SIGNAL(triggered()), this, SLOT(close()));

    connect(this, SIGNAL(connectedToLdapServer()), this, SLOT(updateNetworkTree()));
}

void MainWindow::initMenuBar(void)
{
    QMenu* menuEntry = menuBar()->addMenu(tr("LdapNetworkAdmin"));
    menuEntry->addAction(actionConnect);
    menuEntry->addAction(actionDisconnect);
    menuEntry->addAction(actionRefresh);
    menuEntry->addSeparator();
    menuEntry->addAction(actionQuit);
}

void MainWindow::initLayout(void)
{
    QVBoxLayout* layout = new QVBoxLayout();

    layout->addWidget(new QLabel(tr("Network tree")));

    viewNetworkTree = new QTreeView();
    viewNetworkTree->setModel(networkTree);
    layout->addWidget(viewNetworkTree);

    QWidget* centralWidget = new QWidget();
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
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

bool MainWindow::connectToLdapServer(const Connection& connection)
{
    if (connection.host.isEmpty() || connection.baseDn.isEmpty() || connection.authDn.isEmpty()) {
        qWarning() << "WARNING: Insufficient connection details given. Aborting connection request.";
        return false;
    }
    QString authPassword;
    if (connection.savePassword) {
        authPassword = connection.authPassword;
    } else {
        DialogPassword dialog { this };
        if (dialog.exec() != QDialog::DialogCode::Accepted) {
            return false;
        }
        authPassword = dialog.getPassword();
    }
    return ldapConnection.bind(connection, authPassword);
}

} /* namespace Flix */
