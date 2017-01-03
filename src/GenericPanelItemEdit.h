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
public:
    GenericPanelItemEdit(const QString& title, QWidget* parent = 0);
    virtual ~GenericPanelItemEdit();

protected:
    QWidget* mainContent;
private:
    QPushButton* buttonDelete;

    virtual void initLayout(void);
};

} /* namespace Flix */

#endif /* SRC_GENERICPANELITEMEDIT_H_ */
