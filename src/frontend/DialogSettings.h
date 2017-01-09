/*
 * DialogSettings.h
 *
 *  Created on: 09.01.2017
 *      Author: felix
 */

#ifndef SRC_FRONTEND_DIALOGSETTINGS_H_
#define SRC_FRONTEND_DIALOGSETTINGS_H_

#include <QtWidgets/QDialog>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <frontend/Settings.h>

namespace Flix {

class DialogSettings: public QDialog {
    Q_OBJECT

public:
    DialogSettings(Settings& settings, QWidget *parent = 0, Qt::WindowFlags flags = 0);
    virtual ~DialogSettings();

protected slots:
    void acceptSettings(void);

private:
    Settings& settings;

    QLineEdit* editDefaultDomainSuffix;
    QLineEdit* editDefaultIpAddressPrefix;

    QPushButton* buttonOk;
    QPushButton* buttonCancel;

    void initLayout(void);
};

} /* namespace Flix */

#endif /* SRC_FRONTEND_DIALOGSETTINGS_H_ */
