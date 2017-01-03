/*
 * PanelDefault.h
 *
 *  Created on: 03.01.2017
 *      Author: felix
 */

#ifndef SRC_PANELDEFAULT_H_
#define SRC_PANELDEFAULT_H_

#include <QtWidgets/QGroupBox>

namespace Flix {

class PanelDefault: public QGroupBox {
    void initLayout(void);
public:
    PanelDefault(QWidget* parent = 0);
    virtual ~PanelDefault();
};

} /* namespace Flix */

#endif /* SRC_PANELDEFAULT_H_ */
