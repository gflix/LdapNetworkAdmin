/*
 * Settings.h
 *
 *  Created on: 09.01.2017
 *      Author: felix
 */

#ifndef SRC_FRONTEND_SETTINGS_H_
#define SRC_FRONTEND_SETTINGS_H_

#include <QtCore/QString>

namespace Flix {

struct SettingsDefaults {
    QString domainSuffix;
    QString ipAddressPrefix;
};

class Settings {
public:
    Settings();
    virtual ~Settings();

    void load(void);
    bool save(void) const;

    const SettingsDefaults& getDefaults(void) const;

    void setDefaults(const SettingsDefaults& defaults);

private:
    SettingsDefaults defaults;
};

} /* namespace Flix */

#endif /* SRC_FRONTEND_SETTINGS_H_ */
