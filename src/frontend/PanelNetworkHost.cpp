/*
 * PanelNetworkHost.cpp
 *
 *  Created on: 04.01.2017
 *      Author: felix
 */

#include <QtCore/QDebug>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <frontend/PanelNetworkHost.h>
#include <frontend/DialogTextInput.h>

namespace Flix {

PanelNetworkHost::PanelNetworkHost(const Settings& settings, QWidget* parent):
    GenericPanelItemEdit(tr("Network host"), parent),
    settings(settings),
    canonicalNames(this)
{
    initLayout();
}

PanelNetworkHost::~PanelNetworkHost()
{
}

void PanelNetworkHost::initLayout(void)
{
    int rowIndex = 0;
    QGridLayout* layout = new QGridLayout();

    layout->addWidget(new QLabel(tr("Host name") + ':'), rowIndex, 0);
    editHostName = new QLineEdit();
    layout->addWidget(editHostName, rowIndex++, 1);

    layout->addWidget(new QLabel(tr("Description") + ':'), rowIndex, 0);
    editDescription = new QLineEdit();
    layout->addWidget(editDescription, rowIndex++, 1);

    layout->addWidget(new QLabel(tr("IP address") + ':'), rowIndex, 0);
    editIpAddress = new QLineEdit();
    layout->addWidget(editIpAddress, rowIndex++, 1);

    checkboxDhcpClient = new QCheckBox(tr("DHCP client"));
    layout->addWidget(checkboxDhcpClient, rowIndex++, 0, 1, 2);
    connect(checkboxDhcpClient, SIGNAL(toggled(bool)), this, SLOT(checkboxDhcpClientToggled(bool)));

    layout->addWidget(new QLabel(tr("MAC address") + ':'), rowIndex, 0);
    editMacAddress = new QLineEdit();
    editMacAddress->setEnabled(false);
    layout->addWidget(editMacAddress, rowIndex++, 1);

    QHBoxLayout* canonicalNamesLayout = new QHBoxLayout();
    canonicalNamesLayout->addWidget(new QLabel(tr("Canonical names") + ':'));
    canonicalNamesLayout->addStretch();
    buttonAddCanonicalName = new QPushButton("+");
    connect(buttonAddCanonicalName, SIGNAL(clicked()), this, SLOT(addCanonicalNameTriggered()));
    canonicalNamesLayout->addWidget(buttonAddCanonicalName);
    buttonDeleteCanonicalName = new QPushButton("-");
    connect(buttonDeleteCanonicalName, SIGNAL(clicked()), this, SLOT(deleteCanonicalNameTriggered()));
    canonicalNamesLayout->addWidget(buttonDeleteCanonicalName);
    layout->addLayout(canonicalNamesLayout, rowIndex++, 0, 1, 2);

    viewCanonicalNames = new QListView();
    viewCanonicalNames->setModel(&canonicalNames);
    layout->addWidget(viewCanonicalNames, rowIndex++, 0, 1, 2);

    mainContent->setLayout(layout);
}

PanelNetworkHostSettings PanelNetworkHost::getSettings(void) const
{
    PanelNetworkHostSettings settings;
    settings.hostName = editHostName->text();
    settings.description = editDescription->text();
    settings.ipAddress = editIpAddress->text();
    if (checkboxDhcpClient->isChecked()) {
        settings.macAddress = editMacAddress->text();
    }
    settings.canonicalNames = canonicalNames.stringList();

    return settings;
}

void PanelNetworkHost::setSettings(const PanelNetworkHostSettings& settings)
{
    editHostName->setText(settings.hostName);
    editDescription->setText(settings.description);
    editIpAddress->setText(settings.ipAddress);
    if (!settings.macAddress.isEmpty()) {
        checkboxDhcpClient->setChecked(true);
        editMacAddress->setText(settings.macAddress);
    } else {
        checkboxDhcpClient->setChecked(false);
        editMacAddress->clear();
    }
    canonicalNames.setStringList(settings.canonicalNames);
}

void PanelNetworkHost::checkboxDhcpClientToggled(bool state)
{
    editMacAddress->setEnabled(state);
}

void PanelNetworkHost::addCanonicalNameTriggered(void)
{
    DialogTextInput dialog { tr("Canonical name"), tr("Enter a fully-qualified domain name:"), settings.getDefaults().domainSuffix, this };
    if (dialog.exec() == QDialog::DialogCode::Accepted) {
        int insertionRow = canonicalNames.rowCount();
        canonicalNames.insertRow(insertionRow);
        QModelIndex insertionIndex = canonicalNames.index(insertionRow);
        canonicalNames.setData(insertionIndex, dialog.getTextInput());
        viewCanonicalNames->setCurrentIndex(insertionIndex);
    }
}

void PanelNetworkHost::deleteCanonicalNameTriggered(void)
{
    const QModelIndex& selectedItem = viewCanonicalNames->currentIndex();
    if (selectedItem.isValid()) {
        canonicalNames.removeRow(selectedItem.row());
    }
}

} /* namespace Flix */
