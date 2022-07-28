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

[[nodiscard]] QSettings& getQSettings()
{
    static QSettings s( QSettings::IniFormat,
                        QSettings::UserScope,
                        application_strings::applicationOrganizationName,
                        application_strings::applicationName );

    return s;
}

[[nodiscard]] std::string getQtCategoryName( const SettingCategory category )
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

[[nodiscard]] QVariant getQtSetting( const SettingCategory category,
                                     const std::string qtSettingName )
{
    getQSettings().beginGroup( getQtCategoryName( category ).c_str() );

    const auto v = getQSettings().value( qtSettingName.c_str() );

    getQSettings().endGroup();

    return v;
}

void saveQtSetting( const SettingCategory category,
                    const std::string qtSettingName,
                    const QVariant value )
{
    getQSettings().beginGroup( getQtCategoryName( category ).c_str() );
    getQSettings().setValue( qtSettingName.c_str(), value );
    getQSettings().endGroup();
}

template <typename Value>[[nodiscard]] bool isValidQVariant( const QVariant v )
{
    auto savedSettingIsValid = v.isValid() && !v.isNull();

    if constexpr ( std::is_same<Value, std::string>::value )
    {
        // Special case for std::string because Qt refuses to recognize it
        savedSettingIsValid = savedSettingIsValid && v.canConvert<QString>();
    }
    else
    {
        savedSettingIsValid = savedSettingIsValid && v.canConvert<Value>();
    }

    if ( savedSettingIsValid )
    {
        return true;
    }
    return false;
}

} // namespace settings
