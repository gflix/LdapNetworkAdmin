/*
 * DialogPassword.cpp
 *
 *  Created on: 01.01.2017
 *      Author: felix
 */

#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <frontend/DialogPassword.h>

namespace Flix {

DialogPassword::DialogPassword(QWidget *parent, Qt::WindowFlags flags):
    QDialog(parent, flags)
{
    setWindowTitle(tr("Password dialog"));
    initLayout();
}

DialogPassword::~DialogPassword()
{
}

QString DialogPassword::getPassword(void) const
{
    return editPassword->text();
}

void DialogPassword::initLayout(void)
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    layout->addWidget(new QLabel(tr("Please enter the password") + ":"));

    editPassword = new QLineEdit();
    editPassword->setEchoMode(QLineEdit::EchoMode::Password);
    layout->addWidget(editPassword);

    QHBoxLayout* layoutDialogButtons = new QHBoxLayout();
    buttonOk = new QPushButton(tr("OK"));
    connect(buttonOk, SIGNAL(clicked()), this, SLOT(accept()));
    layoutDialogButtons->addWidget(buttonOk);
    buttonCancel = new QPushButton(tr("Cancel"));
    connect(buttonCancel, SIGNAL(clicked()), this, SLOT(reject()));
    layoutDialogButtons->addWidget(buttonCancel);

    layout->addLayout(layoutDialogButtons);
}

} /* namespace Flix */
