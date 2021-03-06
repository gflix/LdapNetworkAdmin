/*
 * DaialogTextInput.cpp
 *
 *  Created on: 02.01.2017
 *      Author: felix
 */

#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <frontend/DialogTextInput.h>

DialogTextInput::DialogTextInput(const QString& title, const QString& prompt, const QString& defaultText, QWidget* parent, Qt::WindowFlags flags):
    QDialog(parent, flags)
{
    setWindowTitle(title);
    initLayout(prompt, defaultText);
}

DialogTextInput::~DialogTextInput()
{
}


QString DialogTextInput::getTextInput(void) const
{
    return editText->text();
}

void DialogTextInput::initLayout(const QString& prompt, const QString& defaultText)
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    layout->addWidget(new QLabel(prompt));

    editText = new QLineEdit();
    editText->setText(defaultText);
    layout->addWidget(editText);

    QHBoxLayout* layoutDialogButtons = new QHBoxLayout();
    buttonOk = new QPushButton(tr("OK"));
    connect(buttonOk, SIGNAL(clicked()), this, SLOT(accept()));
    layoutDialogButtons->addWidget(buttonOk);
    buttonCancel = new QPushButton(tr("Cancel"));
    connect(buttonCancel, SIGNAL(clicked()), this, SLOT(reject()));
    layoutDialogButtons->addWidget(buttonCancel);

    layout->addLayout(layoutDialogButtons);

    editText->setFocus();
}
