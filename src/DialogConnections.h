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

namespace Flix {

class DialogConnections: public QDialog {
    Q_OBJECT

    void initLayout(void);

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
    QPushButton* buttonDeleteConnection;
public:
    DialogConnections(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    virtual ~DialogConnections();

public slots:
    void newConnection(void);
};

} /* namespace Flix */

#endif /* SRC_DIALOGCONNECTIONS_H_ */
