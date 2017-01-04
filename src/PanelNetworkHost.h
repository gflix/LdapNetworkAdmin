/*
 * PanelNetworkHost.h
 *
 *  Created on: 04.01.2017
 *      Author: felix
 */

#ifndef SRC_PANELNETWORKHOST_H_
#define SRC_PANELNETWORKHOST_H_

#include <QtWidgets/QLineEdit>
#include <GenericPanelItemEdit.h>

namespace Flix {

class PanelNetworkHost: public GenericPanelItemEdit {
    Q_OBJECT

public:
    PanelNetworkHost(QWidget* parent = 0);
    virtual ~PanelNetworkHost();

    QString getHostName(void) const;
    QString getIpAddress(void) const;
    void setHostName(const QString& hostName);
    void setIpAddress(const QString& ipAddress);

private:
    QLineEdit* editHostName;
    QLineEdit* editIpAddress;

    virtual void initLayout(void);
};

} /* namespace Flix */

#endif /* SRC_PANELNETWORKHOST_H_ */
