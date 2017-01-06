/*
 * PanelNetworkHost.h
 *
 *  Created on: 04.01.2017
 *      Author: felix
 */

#ifndef SRC_PANELNETWORKHOST_H_
#define SRC_PANELNETWORKHOST_H_

#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLineEdit>
#include <GenericPanelItemEdit.h>

namespace Flix {

struct PanelNetworkHostSettings {
    QString hostName;
    QString description;
    QString ipAddress;
    QString macAddress;
};

class PanelNetworkHost: public GenericPanelItemEdit {
    Q_OBJECT

public:
    PanelNetworkHost(QWidget* parent = 0);
    virtual ~PanelNetworkHost();

    PanelNetworkHostSettings getSettings(void) const;
    void setSettings(const PanelNetworkHostSettings& settings);

private:
    QLineEdit* editHostName;
    QLineEdit* editDescription;
    QLineEdit* editIpAddress;
    QCheckBox* checkboxDhcpClient;
    QLineEdit* editMacAddress;

    virtual void initLayout(void);

protected slots:
    void checkboxDhcpClientToggled(bool);
};

} /* namespace Flix */

#endif /* SRC_PANELNETWORKHOST_H_ */
