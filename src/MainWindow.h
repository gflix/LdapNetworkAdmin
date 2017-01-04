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
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTreeView>
#include <Common.h>
#include <LdapConnection.h>
#include <ModelNetworkTree.h>
#include <PanelDefault.h>
#include <PanelOrganizationalUnit.h>

namespace Flix {

class MainWindow: public QMainWindow {
    Q_OBJECT

    QAction* actionConnect;
    QAction* actionDisconnect;
    QAction* actionRefresh;
    QAction* actionQuit;

    QAction* actionAddOrganizationalUnit;

    QTreeView* viewNetworkTree;
    ModelNetworkTree* networkTree;

    PanelDefault *panelDefault;
    PanelOrganizationalUnit *panelOrganizationalUnit;

    QStackedWidget* stackedPanels;

    LdapConnection ldapConnection;

    void initActions(void);
    void initMenuBar(void);
    void initLayout(void);
    void setWindowTitleWithState(const QString& state = QString());
    void setupPanelOrganizationalUnit(const LdapObject& object);

    bool connectToLdapServer(const Connection& connection);
public:
    MainWindow();
    virtual ~MainWindow();

signals:
    void connectedToLdapServer(void);

public slots:
    void showConnectionsDialog(void);
    void disconnectFromLdapServer(void);
    void addOrganizationalUnit(void);
    void selectNetworkTreeItem(const QModelIndex& index);
    void deleteNetworkTreeItem(void);
    void updateOrganizationalUnit(void);

protected slots:
    void updateNetworkTree(void);
    void networkTreeCollapsed(const QModelIndex& index);
    void networkTreeExpanded(const QModelIndex& index);
};

} /* namespace Flix */

#endif /* SRC_MAINWINDOW_H_ */
