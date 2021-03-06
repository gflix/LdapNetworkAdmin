/*
 * GenericPanelItemEdit.cpp
 *
 *  Created on: 03.01.2017
 *      Author: felix
 */

#include <QtCore/QDebug>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <frontend/GenericPanelItemEdit.h>

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
    QVBoxLayout* layout = new QVBoxLayout();

    mainContent = new QWidget();
    layout->addWidget(mainContent);

    QHBoxLayout* layoutButtons = new QHBoxLayout();
    buttonSave = new QPushButton(tr("Save"));
    connect(buttonSave, SIGNAL(clicked()), this, SLOT(clickedSave()));
    layoutButtons->addWidget(buttonSave);
    buttonDelete = new QPushButton(tr("Delete"));
    connect(buttonDelete, SIGNAL(clicked()), this, SLOT(clickedDelete()));
    layoutButtons->addWidget(buttonDelete);

    layout->addLayout(layoutButtons);

    setLayout(layout);
}

void GenericPanelItemEdit::clickedSave(void)
{
    emit triggeredSave();
}

void GenericPanelItemEdit::clickedDelete(void)
{
    emit triggeredDelete();
}

} /* namespace Flix */
