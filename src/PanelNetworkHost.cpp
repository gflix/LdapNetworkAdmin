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
    QGridLayout* layout = new QGridLayout();

    layout->addWidget(new QLabel(tr("Host name") + ':'), 0, 0);
    editHostName = new QLineEdit();
    layout->addWidget(editHostName, 0, 1);

    layout->setRowStretch(1, 1);

    mainContent->setLayout(layout);
}

QString PanelNetworkHost::getHostName(void) const
{
    return editHostName->text();
}

void PanelNetworkHost::setHostName(const QString& hostName)
{
    editHostName->setText(hostName);
}

} /* namespace Flix */
