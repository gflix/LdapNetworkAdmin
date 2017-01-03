/*
 * MainWindow.cpp
 *
 *  Created on: 31.12.2016
 *      Author: felix
 */

#include <QtCore/QDebug>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMessageBox>
#include <MainWindow.h>
#include <DialogConnections.h>
#include <DialogPassword.h>
#include <DialogTextInput.h>
#include <LdapTags.h>

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
                QMessageBox::critical(this, tr("Error"), tr("Could not connect to LDAP server!"));
            } else {
                setWindowTitleWithState(connection.name);
                actionRefresh->setEnabled(true);
                actionDisconnect->setEnabled(true);
                emit connectedToLdapServer();
            }
        }
    }
}

void MainWindow::disconnectFromLdapServer(void)
{
    ldapConnection.unbind();
    networkTree->clear();
    setWindowTitleWithState();
    actionRefresh->setEnabled(false);
    actionDisconnect->setEnabled(false);
}

void MainWindow::addOrganizationalUnit(void)
{
    const QModelIndex& index = viewNetworkTree->currentIndex();
    NetworkTreeItem* item = networkTree->getItem(index);
    if (!ldapConnection.isBound() || !index.isValid() || !item->isContainerObject()) {
        return;
    }

    DialogTextInput dialog { tr("Organizational unit"), tr("Enter the name of the new organizational unit") + ':', this };
    if (dialog.exec() == QDialog::DialogCode::Accepted) {
        LdapObject object = LdapObject::createOrganizationalUnit(joinDistinguishedName({ item->getObject().getDistinguishedName(), "ou=" + dialog.getTextInput() }));
        if (!ldapConnection.addObject(object)) {
            QMessageBox::critical(this, tr("Error"), tr("Could not add the organizational unit") + '!');
        }
    }
}

void MainWindow::selectNetworkTreeItem(const QModelIndex& index)
{
    if (!index.isValid()) {
        return;
    }
    NetworkTreeItem* item = networkTree->getItem(index);
    if (!item) {
        return;
    }

    if (item->isOrganizationalUnit()) {
        setupPanelOrganizationalUnit(item->getObject());
        stackedPanels->setCurrentWidget(panelOrganizationalUnit);
    } else {
        stackedPanels->setCurrentWidget(panelDefault);
    }
}

void MainWindow::deleteNetworkTreeItem(void)
{
    qDebug() << "MainWindow::deleteNetworkTreeItem()";
}

void MainWindow::updateNetworkTree(void)
{
    networkTree->clear();
    if (!ldapConnection.isBound()) {
        return;
    }

    const Connection& connection = ldapConnection.getConnection();
    LdapObjects objects;
    if (ldapConnection.searchObjects(objects, joinDistinguishedName({connection.baseDn, connection.subOu}), LdapSearchScope::BASE) && objects.size() == 1) {
        for (auto& object: objects) {
            networkTree->addChild(object, QModelIndex());
        }
    }
}

void MainWindow::networkTreeExpanded(const QModelIndex& index)
{
    if (!index.isValid() || !ldapConnection.isBound()) {
        return;
    }
    NetworkTreeItem* item = networkTree->getItem(index);
    if (!item) {
        return;
    }
    const LdapObject& searchBaseObject = item->getObject();
    LdapObjects objects;
    if (ldapConnection.searchObjects(objects, searchBaseObject.getDistinguishedName(), LdapSearchScope::ONE)) {
        for (auto& object: objects) {
            networkTree->addChild(object, index);
        }
    }
}

void MainWindow::networkTreeCollapsed(const QModelIndex& index)
{
    if (!index.isValid()) {
        return;
    }
    networkTree->deleteTree(index);
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

    actionAddOrganizationalUnit = new QAction(tr("Organizational &unit"), this);
    connect(actionAddOrganizationalUnit, SIGNAL(triggered()), this, SLOT(addOrganizationalUnit()));

    connect(this, SIGNAL(connectedToLdapServer()), this, SLOT(updateNetworkTree()));
}

void MainWindow::initMenuBar(void)
{
    QMenu* menuEntry = menuBar()->addMenu(tr("LdapNetworkAdmin"));
    menuEntry->addAction(actionConnect);
    menuEntry->addAction(actionDisconnect);
    menuEntry->addAction(actionRefresh);
    menuEntry->addSeparator();

    QMenu* subMenu = new QMenu(tr("&New"));
    subMenu->addAction(actionAddOrganizationalUnit);
    menuEntry->addMenu(subMenu);

    menuEntry->addSeparator();
    menuEntry->addAction(actionQuit);
}

void MainWindow::initLayout(void)
{
    QGridLayout* layout = new QGridLayout();

    layout->addWidget(new QLabel(tr("Network tree")), 0, 0);

    viewNetworkTree = new QTreeView();
    viewNetworkTree->setHeaderHidden(true);
    viewNetworkTree->setModel(networkTree);
    connect(viewNetworkTree, SIGNAL(collapsed(const QModelIndex&)), this, SLOT(networkTreeCollapsed(const QModelIndex&)));
    connect(viewNetworkTree, SIGNAL(expanded(const QModelIndex&)), this, SLOT(networkTreeExpanded(const QModelIndex&)));
    connect(viewNetworkTree, SIGNAL(clicked(const QModelIndex&)), this, SLOT(selectNetworkTreeItem(const QModelIndex&)));
    layout->addWidget(viewNetworkTree, 1, 0);

    QHBoxLayout* layoutButtons = new QHBoxLayout();
    QPushButton* button = new QPushButton(tr("New"));
    QMenu* subMenu = new QMenu();
    subMenu->addAction(actionAddOrganizationalUnit);
    button->setMenu(subMenu);
    layoutButtons->addWidget(button);
    layout->addLayout(layoutButtons, 2, 0);

    panelDefault = new PanelDefault();
    panelOrganizationalUnit = new PanelOrganizationalUnit();
    connect(panelOrganizationalUnit, SIGNAL(triggeredDelete()), SLOT(deleteNetworkTreeItem()));

    stackedPanels = new QStackedWidget();
    stackedPanels->addWidget(panelDefault);
    stackedPanels->addWidget(panelOrganizationalUnit);

    layout->addWidget(stackedPanels, 0, 1, 3, 1);

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

void MainWindow::setupPanelOrganizationalUnit(const LdapObject& object)
{
    const LdapAttributeValues& values = object.getAttribute(LDAP_ATTRIBUTE_ORGANIZATIONAL_UNIT);
    if (values.empty()) {
        panelOrganizationalUnit->setOrganizationalUnit(QString());
    } else {
        panelOrganizationalUnit->setOrganizationalUnit(values[0]);
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
