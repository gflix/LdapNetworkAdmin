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

    layout->addWidget(new QLabel(tr("IP address") + ':'), rowIndex, 0);
    editIpAddress = new QLineEdit();
    layout->addWidget(editIpAddress, rowIndex++, 1);

    layout->setRowStretch(rowIndex++, 1);

    mainContent->setLayout(layout);
}

QString PanelNetworkHost::getHostName(void) const
{
    return editHostName->text();
}

QString PanelNetworkHost::getIpAddress(void) const
{
    return editIpAddress->text();
}

void PanelNetworkHost::setHostName(const QString& hostName)
{
    editHostName->setText(hostName);
}

void PanelNetworkHost::setIpAddress(const QString& ipAddress)
{
    editIpAddress->setText(ipAddress);
}

} /* namespace Flix */
