/*
 * PanelDefault.cpp
 *
 *  Created on: 03.01.2017
 *      Author: felix
 */

#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <frontend/PanelDefault.h>

namespace Flix {

PanelDefault::PanelDefault(QWidget* parent):
    QGroupBox(parent)
{
    initLayout();
}

PanelDefault::~PanelDefault()
{
}

void PanelDefault::initLayout(void)
{
    QVBoxLayout* layout = new QVBoxLayout();

    layout->addWidget(new QLabel(tr("Choose an editable element from the tree")));

    setLayout(layout);
}

} /* namespace Flix */
