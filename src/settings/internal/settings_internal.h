#pragma once
#include <QSettings>
#include <string>
#include <type_traits>
#include "../../overlaycontroller.h"

namespace settings
{
enum class SettingCategory
{
    Audio,
    Utility,
    KeyboardShortcut,
    Playspace,
    Application,
    Video,
    Chaperone,
    ChaperoneProfiles,
    Rotation,
    SteamVR,
};

struct QtInfo
{
    const std::string settingName;
};

[[nodiscard]] inline QSettings& getQSettings()
{
    static QSettings settings( QSettings::IniFormat,
                               QSettings::UserScope,
                               application_strings::applicationOrganizationName,
                               application_strings::applicationName );

    return settings;
}

[[nodiscard]] inline std::string
    getQtCategoryName( const SettingCategory category )
{
    switch ( category )
    {
    case SettingCategory::Audio:
        return "audioSettings";
    case SettingCategory::Utility:
        return "utilitiesSettings";
    case SettingCategory::KeyboardShortcut:
        return "keyboardShortcuts";
    case SettingCategory::Playspace:
        return "playspaceSettings";
    case SettingCategory::Application:
        return "applicationSettings";
    case SettingCategory::Video:
        return "videoSettings";
    case SettingCategory::Chaperone:
        return "chaperoneSettings";
    case SettingCategory::ChaperoneProfiles:
        return "chaperoneProfiles";
    case SettingCategory::Rotation:
        return "rotationSettings";
    case SettingCategory::SteamVR:
        return "steamVRSettings";
    }
    return "no-value";
}

[[nodiscard]] inline QVariant getQtSetting( const SettingCategory category,
                                            const std::string qtSettingName )
{
    getQSettings().beginGroup( getQtCategoryName( category ).c_str() );

    auto val = getQSettings().value( qtSettingName.c_str() );

    getQSettings().endGroup();

    return val;
}

inline void saveQtSetting( const SettingCategory category,
                           const std::string qtSettingName,
                           const QVariant value )
{
    getQSettings().beginGroup( getQtCategoryName( category ).c_str() );
    getQSettings().setValue( qtSettingName.c_str(), value );
    getQSettings().endGroup();
}

template <typename Value>
[[nodiscard]] bool isValidQVariant( const QVariant var )
{
    auto savedSettingIsValid = var.isValid() && !var.isNull();

    if constexpr ( std::is_same_v<Value, std::string> )
    {
        // Special case for std::string because Qt refuses to recognize it
        savedSettingIsValid = savedSettingIsValid && var.canConvert<QString>();
    }
    else
    {
        savedSettingIsValid = savedSettingIsValid && var.canConvert<Value>();
    }

    return savedSettingIsValid;
}

} // namespace settings
