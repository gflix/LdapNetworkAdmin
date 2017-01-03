/*
 * PanelOrganizationalUnit.cpp
 *
 *  Created on: 03.01.2017
 *      Author: felix
 */

#include <QtCore/QDebug>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <PanelOrganizationalUnit.h>

namespace Flix {

PanelOrganizationalUnit::PanelOrganizationalUnit(QWidget* parent):
    GenericPanelItemEdit(tr("Organizational unit"), parent)
{
    initLayout();
}

PanelOrganizationalUnit::~PanelOrganizationalUnit()
{
}

void PanelOrganizationalUnit::initLayout(void)
{
    QGridLayout* layout = new QGridLayout();

    layout->addWidget(new QLabel(tr("Identifier") + ':'), 0, 0);
    editOrganizationalUnit = new QLineEdit();
    layout->addWidget(editOrganizationalUnit, 0, 1);

    mainContent->setLayout(layout);
}

QString PanelOrganizationalUnit::getOrganizationalUnit(void) const
{
    return editOrganizationalUnit->text();
}

void PanelOrganizationalUnit::setOrganizationalUnit(const QString& organizationalUnit)
{
    editOrganizationalUnit->setText(organizationalUnit);
}

} /* namespace Flix */
