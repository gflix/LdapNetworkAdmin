/*
 * DialogSettings.cpp
 *
 *  Created on: 09.01.2017
 *      Author: felix
 */

#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <frontend/DialogSettings.h>

namespace Flix {

DialogSettings::DialogSettings(Settings& settings, QWidget *parent, Qt::WindowFlags flags):
    QDialog(parent, flags),
    settings(settings)
{
    setWindowTitle(tr("Settings"));
    initLayout();
}

DialogSettings::~DialogSettings()
{
}

void DialogSettings::acceptSettings(void)
{
    SettingsDefaults defaults;

    defaults.domainSuffix = editDefaultDomainSuffix->text();
    defaults.ipAddressPrefix = editDefaultIpAddressPrefix->text();

    settings.setDefaults(defaults);

    emit accept();
}

void DialogSettings::initLayout(void)
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    QGroupBox* groupboxDefaults = new QGroupBox(tr("Defaults"));
    QGridLayout* layoutDefaults = new QGridLayout(groupboxDefaults);

    int lineIndex = 0;

    layoutDefaults->addWidget(new QLabel(tr("Domain suffix")), lineIndex, 0);
    editDefaultDomainSuffix = new QLineEdit();
    editDefaultDomainSuffix->setText(settings.getDefaults().domainSuffix);
    layoutDefaults->addWidget(editDefaultDomainSuffix, lineIndex++, 1);

    layoutDefaults->addWidget(new QLabel(tr("IP address prefix")), lineIndex, 0);
    editDefaultIpAddressPrefix = new QLineEdit();
    editDefaultIpAddressPrefix->setText(settings.getDefaults().ipAddressPrefix);
    layoutDefaults->addWidget(editDefaultIpAddressPrefix, lineIndex++, 1);

    layout->addWidget(groupboxDefaults);

    QHBoxLayout* layoutDialogButtons = new QHBoxLayout();
    buttonOk = new QPushButton(tr("OK"));
    connect(buttonOk, SIGNAL(clicked()), this, SLOT(acceptSettings()));
    layoutDialogButtons->addWidget(buttonOk);
    buttonCancel = new QPushButton(tr("Cancel"));
    connect(buttonCancel, SIGNAL(clicked()), this, SLOT(reject()));
    layoutDialogButtons->addWidget(buttonCancel);

    layout->addLayout(layoutDialogButtons);
}

} /* namespace Flix */
