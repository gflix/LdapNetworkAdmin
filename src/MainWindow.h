/*
 * MainWindow.h
 *
 *  Created on: 31.12.2016
 *      Author: felix
 */

#ifndef SRC_MAINWINDOW_H_
#define SRC_MAINWINDOW_H_

#include <QtWidgets/QAction>
#include <QtWidgets/QMainWindow>
#include <Common.h>
#include <LdapConnection.h>

namespace Flix {

class MainWindow: public QMainWindow {
    Q_OBJECT

    QAction* actionConnect;
    QAction* actionDisconnect;
    QAction* actionQuit;

    LdapConnection ldapConnection;

    void initActions(void);
    void initMenuBar(void);
    void initLayout(void);
    void setWindowTitleWithState(const QString& state = QString());

    void connectToLdapServer(const Connection& connection);
public:
    MainWindow();
    virtual ~MainWindow();

protected slots:
    void updateNetworkTree(void);

public slots:
    void showConnectionsDialog(void);
    void disconnectFromLdapServer(void);
};

} /* namespace Flix */

#endif /* SRC_MAINWINDOW_H_ */
