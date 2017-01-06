/*
 * PanelNetworkHost.cpp
 *
 *  Created on: 04.01.2017
 *      Author: felix
 */

#include <QtCore/QDebug>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <PanelNetworkHost.h>

namespace Flix {

PanelNetworkHost::PanelNetworkHost(QWidget* parent):
    GenericPanelItemEdit(tr("Network host"), parent)
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

    layout->setRowStretch(rowIndex++, 1);

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
}

void PanelNetworkHost::checkboxDhcpClientToggled(bool state)
{
    editMacAddress->setEnabled(state);
}

} /* namespace Flix */
