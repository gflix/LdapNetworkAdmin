/*
 * GenericPanelItemEdit.cpp
 *
 *  Created on: 03.01.2017
 *      Author: felix
 */

#include <QtCore/QDebug>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <GenericPanelItemEdit.h>

namespace Flix {

GenericPanelItemEdit::GenericPanelItemEdit(const QString& title, QWidget* parent):
    QGroupBox(title, parent)
{
    initLayout();
}

GenericPanelItemEdit::~GenericPanelItemEdit()
{
}

void GenericPanelItemEdit::initLayout(void)
{
    qDebug() << "GenericPanelItemEdit::initLayout()";

    QVBoxLayout* layout = new QVBoxLayout();

    mainContent = new QWidget();
    layout->addWidget(mainContent);

    QHBoxLayout* layoutButtons = new QHBoxLayout();
    buttonDelete = new QPushButton(tr("Delete"));
    layoutButtons->addWidget(buttonDelete);

    layout->addLayout(layoutButtons);

    setLayout(layout);
}

} /* namespace Flix */
