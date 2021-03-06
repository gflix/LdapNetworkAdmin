/*
 * DialogConnections.h
 *
 *  Created on: 31.12.2016
 *      Author: felix
 */

#ifndef SRC_DIALOGCONNECTIONS_H_
#define SRC_DIALOGCONNECTIONS_H_

#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTreeView>
#include <frontend/ModelConnections.h>

namespace Flix {

class DialogConnections: public QDialog {
    Q_OBJECT

    QLineEdit* editConnectionName;
    QLineEdit* editHost;
    QLineEdit* editPort;
    QLineEdit* editBaseDn;
    QLineEdit* editAuthDn;
    QLineEdit* editAuthPassword;
    QLineEdit* editSubOu;

    QCheckBox* checkboxSaveAuthPassword;

    QPushButton* buttonNewConnection;
    QPushButton* buttonSaveConnection;
    QPushButton* buttonSaveAsNewConnection;
    QPushButton* buttonDeleteConnection;
    QPushButton* buttonConnect;
    QPushButton* buttonCancel;

    QTreeView* viewConnections;
    ModelConnections* connections;

    void initLayout(void);
    void resetConnectionsView(void);
public:
    DialogConnections(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    virtual ~DialogConnections();

    bool getConnection(Connection& connection) const;

public slots:
    void newConnection(void);
    void saveConnection(void);
    void saveAsNewConnection(void);
    void deleteConnection(void);
    void selectConnection(const QModelIndex& index);
    void selectConnectionAndAccept(const QModelIndex& index);
};

} /* namespace Flix */

#endif /* SRC_DIALOGCONNECTIONS_H_ */
