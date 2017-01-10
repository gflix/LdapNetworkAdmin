/*
 * PanelNetworkHost.h
 *
 *  Created on: 04.01.2017
 *      Author: felix
 */

#ifndef SRC_PANELNETWORKHOST_H_
#define SRC_PANELNETWORKHOST_H_

#include <QtCore/QStringListModel>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <frontend/GenericPanelItemEdit.h>
#include <common/Common.h>
#include <frontend/Settings.h>

namespace Flix {

struct PanelNetworkHostSettings {
    QString hostName;
    QString description;
    QString ipAddress;
    QString macAddress;
    CanonicalNames canonicalNames;
};

class PanelNetworkHost: public GenericPanelItemEdit {
    Q_OBJECT

public:
    PanelNetworkHost(const Settings& settings, QWidget* parent = 0);
    virtual ~PanelNetworkHost();

    PanelNetworkHostSettings getSettings(void) const;
    void setSettings(const PanelNetworkHostSettings& settings);

private:
    const Settings& settings;

    QLineEdit* editHostName;
    QLineEdit* editDescription;
    QLineEdit* editIpAddress;
    QCheckBox* checkboxDhcpClient;
    QLineEdit* editMacAddress;
    QListView* viewCanonicalNames;
    QPushButton* buttonAddCanonicalName;
    QPushButton* buttonDeleteCanonicalName;

    QStringListModel canonicalNames;

    virtual void initLayout(void);

protected slots:
    void checkboxDhcpClientToggled(bool);
    void addCanonicalNameTriggered(void);
    void deleteCanonicalNameTriggered(void);
};

} /* namespace Flix */

#endif /* SRC_PANELNETWORKHOST_H_ */
