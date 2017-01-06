/*
 * DialogPassword.h
 *
 *  Created on: 01.01.2017
 *      Author: felix
 */

#ifndef SRC_DIALOGPASSWORD_H_
#define SRC_DIALOGPASSWORD_H_

#include <QtWidgets/QDialog>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

namespace Flix {

class DialogPassword: public QDialog {
    Q_OBJECT

    QLineEdit* editPassword;

    QPushButton* buttonOk;
    QPushButton* buttonCancel;

    void initLayout(void);
public:
    DialogPassword(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    virtual ~DialogPassword();

    QString getPassword(void) const;
};

} /* namespace Flix */

#endif /* SRC_DIALOGPASSWORD_H_ */
