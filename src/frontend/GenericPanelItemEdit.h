/*
 * GenericPanelItemEdit.h
 *
 *  Created on: 03.01.2017
 *      Author: felix
 */

#ifndef SRC_GENERICPANELITEMEDIT_H_
#define SRC_GENERICPANELITEMEDIT_H_

#include <QtWidgets/QGroupBox>
#include <QtWidgets/QPushButton>

namespace Flix {

class GenericPanelItemEdit: public QGroupBox {
    Q_OBJECT

public:
    GenericPanelItemEdit(const QString& title, QWidget* parent = 0);
    virtual ~GenericPanelItemEdit();

signals:
    void triggeredSave(void);
    void triggeredDelete(void);

protected:
    QWidget* mainContent;
private:
    QPushButton* buttonSave;
    QPushButton* buttonDelete;

    virtual void initLayout(void);

protected slots:
    void clickedSave(void);
    void clickedDelete(void);

};

} /* namespace Flix */

#endif /* SRC_GENERICPANELITEMEDIT_H_ */
