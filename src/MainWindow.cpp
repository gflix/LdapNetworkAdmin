/*
 * MainWindow.cpp
 *
 *  Created on: 31.12.2016
 *      Author: felix
 */

#include <QtWidgets/QMenuBar>
#include <MainWindow.h>
#include <DialogConnections.h>

namespace Flix {

MainWindow::MainWindow()
{
    setWindowTitle(tr("LDAP network admin"));
    initActions();
    initMenuBar();
}

MainWindow::~MainWindow()
{
}

void MainWindow::showConnectionsDialog(void)
{
    DialogConnections dialog { this };
    if (dialog.exec() == QDialog::DialogCode::Accepted) {
        //
    }
}

void MainWindow::initActions(void)
{
    actionConnect = new QAction(tr("&Connect..."), this);
    actionConnect->setShortcut(Qt::CTRL | Qt::Key_C);
    connect(actionConnect, SIGNAL(triggered()), this, SLOT(showConnectionsDialog()));

    actionDisconnect = new QAction(tr("&Disconnect"), this);
    actionDisconnect->setShortcut(Qt::CTRL | Qt::Key_D);
    actionDisconnect->setEnabled(false);

    actionQuit = new QAction(tr("&Quit"), this);
    actionQuit->setShortcut(Qt::CTRL | Qt::Key_Q);
    connect(actionQuit, SIGNAL(triggered()), this, SLOT(close()));
}

void MainWindow::initMenuBar(void)
{
    QMenu* menuEntry = menuBar()->addMenu(tr("LdapNetworkAdmin"));
    menuEntry->addAction(actionConnect);
    menuEntry->addAction(actionDisconnect);
    menuEntry->addSeparator();
    menuEntry->addAction(actionQuit);
}

} /* namespace Flix */
