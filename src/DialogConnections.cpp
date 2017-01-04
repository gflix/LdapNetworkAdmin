/*
 * DialogConnections.cpp
 *
 *  Created on: 31.12.2016
 *      Author: felix
 */

#include <QtCore/QDebug>
#include <QtGui/QIntValidator>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <DialogConnections.h>

#define DEFAULT_LDAP_PORT "389"

namespace Flix {

DialogConnections::DialogConnections(QWidget *parent, Qt::WindowFlags flags):
    QDialog(parent, flags)
{
    setWindowTitle("Connect to LDAP server");

    connections = new ModelConnections(this);

    initLayout();
    resetConnectionsView();
}

DialogConnections::~DialogConnections()
{
}

bool DialogConnections::getConnection(Connection& connection) const
{
    connection.name = editConnectionName->text();
    connection.host = editHost->text();
    connection.port = editPort->text().toInt(nullptr, 10);
    connection.baseDn = editBaseDn->text();
    connection.authDn = editAuthDn->text();
    connection.authPassword = editAuthPassword->text();
    connection.savePassword = checkboxSaveAuthPassword->isChecked();
    connection.subOu = editSubOu->text();

    if (connection.name.isEmpty()) {
        connection.name = connection.host;
    }
    if (!connection.savePassword) {
        connection.authPassword.clear();
    }

    return true;
}

void DialogConnections::newConnection(void)
{
    resetConnectionsView();

    editConnectionName->clear();
    editHost->clear();
    editPort->setText(DEFAULT_LDAP_PORT);
    editBaseDn->clear();
    editAuthDn->clear();
    editAuthPassword->clear();
    editSubOu->clear();
}

void DialogConnections::saveConnection(void)
{
    Connection connection;
    if (!getConnection(connection)) {
        return;
    }

    const QModelIndex& index = viewConnections->currentIndex();
    if (!index.isValid()) {
        connections->addConnection(connection);
    } else {
        connections->updateConnection(index, connection);
    }
}

void DialogConnections::deleteConnection(void)
{
    const QModelIndex& index = viewConnections->currentIndex();
    if (!index.isValid()) {
        return;
    }
    connections->deleteConnection(index);
    resetConnectionsView();
}

void DialogConnections::selectConnection(const QModelIndex& index)
{
    const Connection& connection = connections->getConnection(index);

    editConnectionName->setText(connection.name);
    editHost->setText(connection.host);
    editPort->setText(QString::number(connection.port));
    editBaseDn->setText(connection.baseDn);
    editAuthDn->setText(connection.authDn);
    checkboxSaveAuthPassword->setChecked(connection.savePassword);
    if (connection.savePassword) {
        editAuthPassword->setText(connection.authPassword);
    } else {
        editAuthPassword->clear();
    }
    editSubOu->setText(connection.subOu);
    buttonDeleteConnection->setEnabled(true);
}

void DialogConnections::selectConnectionAndAccept(const QModelIndex& index)
{
    selectConnection(index);

    emit accept();
}

void DialogConnections::initLayout(void)
{
    int lineIndex = 0;
    QGridLayout* layout = new QGridLayout(this);

    layout->addWidget(new QLabel(tr("Available connections") + ":"), 0, 0);

    layout->addWidget(new QLabel(tr("Connection name") + ":"), lineIndex, 1, Qt::AlignRight);
    editConnectionName = new QLineEdit();
    layout->addWidget(editConnectionName, lineIndex++, 2);

    layout->addWidget(new QLabel(tr("Host") + ":"), lineIndex, 1, Qt::AlignRight);
    editHost = new QLineEdit();
    layout->addWidget(editHost, lineIndex, 2);
    layout->addWidget(new QLabel(tr("Port") + ":"), lineIndex, 3, Qt::AlignRight);
    editPort = new QLineEdit(DEFAULT_LDAP_PORT);
    editPort->setValidator(new QIntValidator(1, 65535));
    layout->addWidget(editPort, lineIndex++, 4);

    layout->addWidget(new QLabel(tr("Base DN") + ":"), lineIndex, 1, Qt::AlignRight);
    editBaseDn = new QLineEdit();
    layout->addWidget(editBaseDn, lineIndex++, 2, 1, 3);

    layout->addWidget(new QLabel(tr("Auth DN") + ":"), lineIndex, 1, Qt::AlignRight);
    editAuthDn = new QLineEdit();
    editAuthDn->setPlaceholderText("cn=admin,dc=...,dc=...");
    layout->addWidget(editAuthDn, lineIndex++, 2, 1, 3);

    layout->addWidget(new QLabel(tr("Password") + ":"), lineIndex, 1, Qt::AlignRight);
    editAuthPassword = new QLineEdit();
    editAuthPassword->setEchoMode(QLineEdit::Password);
    layout->addWidget(editAuthPassword, lineIndex, 2);
    checkboxSaveAuthPassword = new QCheckBox(tr("Save password"));
    layout->addWidget(checkboxSaveAuthPassword, lineIndex++, 3, 1, 2);

    layout->addWidget(new QLabel(tr("Sub OU") + ":"), lineIndex, 1, Qt::AlignRight);
    editSubOu = new QLineEdit();
    layout->addWidget(editSubOu, lineIndex++, 2, 1, 3);

    viewConnections = new QTreeView();
    viewConnections->setModel(connections);
    connect(viewConnections, SIGNAL(clicked(const QModelIndex&)), this, SLOT(selectConnection(const QModelIndex&)));
    connect(viewConnections, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(selectConnectionAndAccept(const QModelIndex&)));
    layout->addWidget(viewConnections, 1, 0, lineIndex - 1, 1);

    QHBoxLayout* layoutConnectionModificationButtons = new QHBoxLayout();
    buttonNewConnection = new QPushButton(tr("New"));
    connect(buttonNewConnection, SIGNAL(clicked()), this, SLOT(newConnection()));
    layoutConnectionModificationButtons->addWidget(buttonNewConnection);
    buttonSaveConnection = new QPushButton(tr("Save"));
    connect(buttonSaveConnection, SIGNAL(clicked()), this, SLOT(saveConnection()));
    layoutConnectionModificationButtons->addWidget(buttonSaveConnection);
    buttonDeleteConnection = new QPushButton(tr("Delete"));
    connect(buttonDeleteConnection, SIGNAL(clicked()), this, SLOT(deleteConnection()));
    layoutConnectionModificationButtons->addWidget(buttonDeleteConnection);

    layout->addLayout(layoutConnectionModificationButtons, lineIndex, 0);

    QHBoxLayout* layoutDialogButtons = new QHBoxLayout();
    buttonConnect = new QPushButton(tr("Connect"));
    connect(buttonConnect, SIGNAL(clicked()), this, SLOT(accept()));
    layoutDialogButtons->addWidget(buttonConnect);
    buttonCancel = new QPushButton(tr("Cancel"));
    connect(buttonCancel, SIGNAL(clicked()), this, SLOT(reject()));
    layoutDialogButtons->addWidget(buttonCancel);

    layout->addLayout(layoutDialogButtons, lineIndex++, 2, 1, 3);
}

void DialogConnections::resetConnectionsView(void)
{
    viewConnections->clearSelection();
    viewConnections->setCurrentIndex(QModelIndex());
    buttonDeleteConnection->setEnabled(false);
}

} /* namespace Flix */
