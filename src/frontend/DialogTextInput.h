/*
 * DaialogTextInput.h
 *
 *  Created on: 02.01.2017
 *      Author: felix
 */

#ifndef SRC_DIALOGTEXTINPUT_H_
#define SRC_DIALOGTEXTINPUT_H_

#include <QtWidgets/QDialog>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

class DialogTextInput: public QDialog {
    Q_OBJECT

    QLineEdit* editText;

    QPushButton* buttonOk;
    QPushButton* buttonCancel;

    void initLayout(const QString& prompt);
public:
    DialogTextInput(const QString& title, const QString& prompt, QWidget *parent = 0, Qt::WindowFlags flags = 0);
    virtual ~DialogTextInput();

    QString getTextInput(void) const;
};

#endif /* SRC_DIALOGTEXTINPUT_H_ */
