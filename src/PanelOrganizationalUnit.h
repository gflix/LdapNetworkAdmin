/*
 * PanelOrganizationalUnit.h
 *
 *  Created on: 03.01.2017
 *      Author: felix
 */

#ifndef SRC_PANELORGANIZATIONALUNIT_H_
#define SRC_PANELORGANIZATIONALUNIT_H_

#include <QtWidgets/QLineEdit>
#include <GenericPanelItemEdit.h>

namespace Flix {

class PanelOrganizationalUnit: public GenericPanelItemEdit {
    Q_OBJECT

public:
    PanelOrganizationalUnit(QWidget* parent = 0);
    virtual ~PanelOrganizationalUnit();

    QString getOrganizationalUnit(void) const;
    void setOrganizationalUnit(const QString& organizationalUnit);

private:
    QLineEdit* editOrganizationalUnit;

    virtual void initLayout(void);
};

} /* namespace Flix */

#endif /* SRC_PANELORGANIZATIONALUNIT_H_ */
