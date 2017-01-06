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
#include <LdapObjectOrganizationalUnit.h>
#include <LdapObjectNetworkHost.h>

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
    GenericLdapObject* parentObject = networkTree->getItem(index)->getObject();
    if (!ldapConnection.isBound() || !index.isValid() || !parentObject || (!parentObject->isObjectType(LdapObjectType::DC_OBJECT) && !parentObject->isObjectType(LdapObjectType::ORGANIZATIONAL_UNIT))) {
        return;
    }

    DialogTextInput dialog { tr("Organizational unit"), tr("Enter the name of the new organizational unit") + ':', this };
    if (dialog.exec() == QDialog::DialogCode::Accepted) {
        // need to store the result in a separate variable, passing the result as an argument may lead to segfault
        QString joinedDistinguishedName { joinDistinguishedName({ parentObject->getDistinguishedName(), "ou=" + dialog.getTextInput() }) };
        GenericLdapObject* object = LdapObjectOrganizationalUnit::create(joinedDistinguishedName);

        if (!ldapConnection.addObject(object)) {
            QMessageBox::critical(this, tr("Error"), tr("Could not add the organizational unit") + '!');
        } else {
            networkTree->addChild(object, index);
        }
    }
}

void MainWindow::addNetworkHost(void)
{
    const QModelIndex& index = viewNetworkTree->currentIndex();
    GenericLdapObject* parentObject = networkTree->getItem(index)->getObject();
    if (!ldapConnection.isBound() || !index.isValid() || !parentObject || (!parentObject->isObjectType(LdapObjectType::DC_OBJECT) && !parentObject->isObjectType(LdapObjectType::ORGANIZATIONAL_UNIT))) {
        return;
    }

    DialogTextInput dialog { tr("Host name"), tr("Enter the fully qualified host name") + ':', this };
    if (dialog.exec() == QDialog::DialogCode::Accepted) {
        // need to store the result in a separate variable, passing the result as an argument may lead to segfault
        QString joinedDistinguishedName { joinDistinguishedName({ parentObject->getDistinguishedName(), "cn=" + dialog.getTextInput() }) };
        GenericLdapObject* object = LdapObjectNetworkHost::create(joinedDistinguishedName, "0.0.0.0");

        if (!ldapConnection.addObject(object)) {
            QMessageBox::critical(this, tr("Error"), tr("Could not add the host") + '!');
        } else {
            networkTree->addChild(object, index);
        }
    }
}

void MainWindow::selectNetworkTreeItem(const QModelIndex& index)
{
    if (!index.isValid()) {
        return;
    }
    GenericLdapObject* object = networkTree->getItem(index)->getObject();

    if (object->isObjectType(LdapObjectType::ORGANIZATIONAL_UNIT)) {
        setupPanelOrganizationalUnit(object);
        stackedPanels->setCurrentWidget(panelOrganizationalUnit);
    } else if (object->isObjectType(LdapObjectType::NETWORK_HOST)) {
        setupPanelNetworkHost(object);
        stackedPanels->setCurrentWidget(panelNetworkHost);
    } else {
        stackedPanels->setCurrentWidget(panelDefault);
    }
}

void MainWindow::deleteNetworkTreeItem(void)
{
    const QModelIndex& index = viewNetworkTree->currentIndex();
    if (!ldapConnection.isBound() || !index.isValid()) {
        return;
    }
    GenericLdapObject* object = networkTree->getItem(index)->getObject();
    if (QMessageBox::question(this, tr("Confirmation"), tr("Really delete the object \"%1\"?").arg(object->getDistinguishedName())) == QMessageBox::Yes) {
        if (!ldapConnection.deleteObject(object)) {
            QMessageBox::warning(this, tr("Warning"), tr("Object was not deleted."));
        }
    }
}

void MainWindow::updateOrganizationalUnit(void)
{
    const QModelIndex& index = viewNetworkTree->currentIndex();
    if (!ldapConnection.isBound() || !index.isValid()) {
        return;
    }
    GenericLdapObject* object = networkTree->getItem(index)->getObject();
    if (object->getIdentifier() != panelOrganizationalUnit->getOrganizationalUnit()) {
        if (!ldapConnection.renameObject(&object, panelOrganizationalUnit->getOrganizationalUnit())) {
            QMessageBox::critical(this, tr("Error"), tr("Could not rename the LDAP object") + '!');
            return;
        }

        bool expandAgain = false;
        if (viewNetworkTree->isExpanded(index)) {
            emit networkTreeCollapsed(index);
            expandAgain = true;
        }
        networkTree->updateItem(object, index);
        if (expandAgain) {
            emit networkTreeExpanded(index);
        }
    }
}

void MainWindow::updateNetworkHost(void)
{
    const QModelIndex& index = viewNetworkTree->currentIndex();
    if (!ldapConnection.isBound() || !index.isValid()) {
        return;
    }
    GenericLdapObject* object = networkTree->getItem(index)->getObject();
    if (!object || !object->isObjectType(LdapObjectType::NETWORK_HOST)) {
        return;
    }
    LdapObjectNetworkHost* objectNetworkHost = (LdapObjectNetworkHost*) object;
    PanelNetworkHostSettings networkHostSettings = panelNetworkHost->getSettings();
    if (objectNetworkHost->getIdentifier() != networkHostSettings.hostName) {
        if (!ldapConnection.renameObject((GenericLdapObject**) (&objectNetworkHost), networkHostSettings.hostName)) {
            QMessageBox::critical(this, tr("Error"), tr("Could not rename the LDAP object") + '!');
            return;
        }

        bool expandAgain = false;
        if (viewNetworkTree->isExpanded(index)) {
            emit networkTreeCollapsed(index);
            expandAgain = true;
        }
        networkTree->updateItem(objectNetworkHost, index);
        if (expandAgain) {
            emit networkTreeExpanded(index);
        }
    }
    if (objectNetworkHost->getIpAddress() != networkHostSettings.ipAddress ||
        objectNetworkHost->getMacAddress() != networkHostSettings.macAddress) {
        objectNetworkHost->setIpAddress(networkHostSettings.ipAddress);
        objectNetworkHost->setMacAddress(networkHostSettings.macAddress);
        if (!ldapConnection.updateObject((GenericLdapObject*) objectNetworkHost)) {
            QMessageBox::critical(this, tr("Error"), tr("Could not update the LDAP object") + '!');
            return;
        }
    }
}

void MainWindow::updateNetworkTree(void)
{
    networkTree->clear();
    stackedPanels->setCurrentWidget(panelDefault);
    if (!ldapConnection.isBound()) {
        return;
    }

    const Connection& connection = ldapConnection.getConnection();
    LdapObjects objects;
    if (ldapConnection.searchObjects(objects, joinDistinguishedName({connection.baseDn, connection.subOu}), LdapSearchScope::BASE) && objects.size() == 1) {
        for (auto& object: objects) {
            networkTree->addChild(GenericLdapObject::getDuplicate(object.get()), QModelIndex());
        }
    }
}

void MainWindow::networkTreeExpanded(const QModelIndex& index)
{
    if (!index.isValid() || !ldapConnection.isBound()) {
        return;
    }
    networkTree->deleteTree(index);
    NetworkTreeItem* item = networkTree->getItem(index);
    if (!item) {
        return;
    }
    GenericLdapObject* searchBaseObject = item->getObject();
    if (!searchBaseObject) {
        return;
    }
    LdapObjects objects;
    if (ldapConnection.searchObjects(objects, searchBaseObject->getDistinguishedName(), LdapSearchScope::ONE)) {
        for (auto& object: objects) {
            networkTree->addChild(GenericLdapObject::getDuplicate(object.get()), index);
        }
    }
}

void MainWindow::networkTreeCollapsed(const QModelIndex& index)
{
    if (!index.isValid()) {
        return;
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

    actionAddOrganizationalUnit = new QAction(tr("Organizational &unit"), this);
    connect(actionAddOrganizationalUnit, SIGNAL(triggered()), this, SLOT(addOrganizationalUnit()));

    actionAddNetworkHost = new QAction(tr("Network &host"), this);
    connect(actionAddNetworkHost, SIGNAL(triggered()), this, SLOT(addNetworkHost()));

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
    subMenu->addAction(actionAddNetworkHost);
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
    subMenu->addAction(actionAddNetworkHost);
    button->setMenu(subMenu);
    layoutButtons->addWidget(button);
    layoutButtons->addStretch();
    layout->addLayout(layoutButtons, 2, 0);

    panelDefault = new PanelDefault();
    panelOrganizationalUnit = new PanelOrganizationalUnit();
    connect(panelOrganizationalUnit, SIGNAL(triggeredSave()), SLOT(updateOrganizationalUnit()));
    connect(panelOrganizationalUnit, SIGNAL(triggeredDelete()), SLOT(deleteNetworkTreeItem()));
    panelNetworkHost = new PanelNetworkHost();
    connect(panelNetworkHost, SIGNAL(triggeredSave()), SLOT(updateNetworkHost()));
    connect(panelNetworkHost, SIGNAL(triggeredDelete()), SLOT(deleteNetworkTreeItem()));

    stackedPanels = new QStackedWidget();
    stackedPanels->addWidget(panelDefault);
    stackedPanels->addWidget(panelOrganizationalUnit);
    stackedPanels->addWidget(panelNetworkHost);

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

void MainWindow::setupPanelOrganizationalUnit(GenericLdapObject* object)
{
    if (!object) {
        return;
    }
    panelOrganizationalUnit->setOrganizationalUnit(object->getIdentifier());
}

void MainWindow::setupPanelNetworkHost(GenericLdapObject* object)
{
    if (!object) {
        return;
    }
    LdapObjectNetworkHost* objectNetworkHost = (LdapObjectNetworkHost*) object;
    PanelNetworkHostSettings networkHostSettings;

    networkHostSettings.hostName = objectNetworkHost->getIdentifier();
    networkHostSettings.ipAddress = objectNetworkHost->getIpAddress();
    networkHostSettings.macAddress = objectNetworkHost->getMacAddress();

    panelNetworkHost->setSettings(networkHostSettings);
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
