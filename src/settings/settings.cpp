#include <utility>
#include <easylogging++.h>
#include "../overlaycontroller.h"
#include "internal/settings_controller.h"

namespace settings
{
static SettingsController settingController{};

void saveChangedSettings()
{
    settingController.saveChangedSettings();
}

void saveAllSettings()
{
    LOG( INFO ) << "Saving all settings.";
    settingController.saveAllSettings();
    LOG( INFO ) << "All settings saved.";
}

[[nodiscard]] bool getSetting( const BoolSetting setting )
{
    return settingController.getSetting<bool>( setting );
}

void setSetting( const BoolSetting setting, const bool value )
{
    settingController.setSetting( setting, value );
}

[[nodiscard]] double getSetting( const DoubleSetting setting )
{
    return settingController.getSetting<double>( setting );
}

void setSetting( const DoubleSetting setting, const double value )
{
    settingController.setSetting( setting, value );
}

[[nodiscard]] int getSetting( const IntSetting setting )
{
    return settingController.getSetting<int>( setting );
}

void setSetting( const IntSetting setting, const int value )
{
    settingController.setSetting( setting, value );
}

[[nodiscard]] std::string getSetting( const StringSetting setting )
{
    return settingController.getSetting<std::string>( setting );
}

void setSetting( const StringSetting setting, const std::string value )
{
    settingController.setSetting( setting, value );
}

std::string initializeAndGetSettingsPath()
{
    // The static object is initialized the first time the function is called.
    // Having a dedicated function for this allows more control over when this
    // happens.
    return settingController.getSettingsFileName();
}

std::string getSettingsAndValues()
{
    return settingController.getSettingsAndValues();
}

} // namespace settings
