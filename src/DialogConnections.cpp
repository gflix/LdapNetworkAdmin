/*
 * DialogConnections.cpp
 *
 *  Created on: 31.12.2016
 *      Author: felix
 */

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
    initLayout();
}

DialogConnections::~DialogConnections()
{
}

void DialogConnections::newConnection(void)
{
    editConnectionName->clear();
    editHost->clear();
    editPort->setText(DEFAULT_LDAP_PORT);
    editBaseDn->clear();
    editAuthDn->clear();
    editAuthPassword->clear();
    editSubOu->clear();
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
    layout->addWidget(editBaseDn, lineIndex++, 2);

    layout->addWidget(new QLabel(tr("Auth DN") + ":"), lineIndex, 1, Qt::AlignRight);
    editAuthDn = new QLineEdit();
    layout->addWidget(editAuthDn, lineIndex++, 2);

    layout->addWidget(new QLabel(tr("Password") + ":"), lineIndex, 1, Qt::AlignRight);
    editAuthPassword = new QLineEdit();
    editAuthPassword->setEchoMode(QLineEdit::Password);
    layout->addWidget(editAuthPassword, lineIndex, 2);
    checkboxSaveAuthPassword = new QCheckBox(tr("Save password"));
    layout->addWidget(checkboxSaveAuthPassword, lineIndex++, 3, 1, 2);

    layout->addWidget(new QLabel(tr("Sub OU") + ":"), lineIndex, 1, Qt::AlignRight);
    editSubOu = new QLineEdit();
    layout->addWidget(editSubOu, lineIndex++, 2);

    QHBoxLayout* layoutConnectionModificationButtons = new QHBoxLayout();
    buttonNewConnection = new QPushButton(tr("New"));
    connect(buttonNewConnection, SIGNAL(clicked()), this, SLOT(newConnection()));
    layoutConnectionModificationButtons->addWidget(buttonNewConnection);
    buttonSaveConnection = new QPushButton(tr("Save"));
    layoutConnectionModificationButtons->addWidget(buttonSaveConnection);
    buttonDeleteConnection = new QPushButton(tr("Delete"));
    layoutConnectionModificationButtons->addWidget(buttonDeleteConnection);

    layout->addLayout(layoutConnectionModificationButtons, lineIndex++, 2, 1, 4);
}

} /* namespace Flix */
