#include <utility>
#include <QSettings>
#include <easylogging++.h>
#include "../overlaycontroller.h"
#include "settings_controller.h"

namespace settings
{
SettingsController& getSettingsController()
{
    static SettingsController s{};

    return s;
}

void saveChangedSettings()
{
    getSettingsController().saveChangedSettings();
}

void saveAllSettings()
{
    LOG( INFO ) << "Saving all settings.";
    getSettingsController().saveAllSettings();
    LOG( INFO ) << "All settings saved.";
}

[[nodiscard]] bool getSetting( const BoolSetting setting )
{
    return getSettingsController().getSetting<bool>( setting );
}

void setSetting( const BoolSetting setting, const bool value )
{
    getSettingsController().setSetting( setting, value );
}

[[nodiscard]] double getSetting( const DoubleSetting setting )
{
    return getSettingsController().getSetting<double>( setting );
}

void setSetting( const DoubleSetting setting, const double value )
{
    getSettingsController().setSetting( setting, value );
}

[[nodiscard]] int getSetting( const IntSetting setting )
{
    return getSettingsController().getSetting<int>( setting );
}

void setSetting( const IntSetting setting, const int value )
{
    getSettingsController().setSetting( setting, value );
}

[[nodiscard]] std::string getSetting( const StringSetting setting )
{
    return getSettingsController().getSetting<std::string>( setting );
}

void setSetting( const StringSetting setting, const std::string value )
{
    getSettingsController().setSetting( setting, value );
}

std::string initializeAndGetSettingsPath()
{
    // The static object is initialized the first time the function is called.
    // Having a dedicated function for this allows more control over when this
    // happens.
    return getSettingsController().getSettingsFileName();
}

} // namespace settings
