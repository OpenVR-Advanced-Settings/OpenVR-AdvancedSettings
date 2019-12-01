#pragma once
#include <assert.h>
#include <array>
#include <vector>
#include <easylogging++.h>
#include "settings.h"
#include "setting_value.h"
#include "../utils/setup.h"
#include "specific_setting_value.h"

namespace settings
{
template <typename Enum, int ArraySize, typename Value>
void verifySettings( std::array<Value, ArraySize> v ) noexcept
{
    for ( int settingIndex = 0; settingIndex < ( ArraySize - 1 );
          ++settingIndex )
    {
        // Having the enum values be in the correct location makes getting a
        // value O(1) instead of O(n) due to array lookup vs traversal
        // Also ensures that there are no mising values

        const auto currentSetting
            = v[static_cast<std::size_t>( settingIndex )].setting();

        const auto settingCorrectlyLocated
            = currentSetting == static_cast<Enum>( settingIndex );
        if ( !settingCorrectlyLocated )
        {
            const auto enumName = typeid( Enum ).name();

            const auto enumSettingNumber = static_cast<int>( currentSetting );

            LOG( ERROR ) << enumName << ": at index '" << settingIndex
                         << "' is incorrect. Current value: '"
                         << enumSettingNumber
                         << "'. Exiting "
                            "with error code "
                         << ReturnErrorCode::SETTING_INCORRECT_INDEX << ".";
            exit( ReturnErrorCode::SETTING_INCORRECT_INDEX );
        }
    }

    LOG( DEBUG ) << "Settings for '" << typeid( Enum ).name() << "' verified.";
}

class SettingsController
{
public:
    SettingsController()
    {
        verifySettings<BoolSetting, boolSettingSize>( m_boolSettings );

        verifySettings<DoubleSetting, doubleSettingSize>( m_doubleSettings );

        verifySettings<StringSetting, stringSettingsSize>( m_stringSettings );

        verifySettings<IntSetting, intSettingsSize>( m_intSettings );
    }

    std::string getSettingsFileName() const noexcept
    {
        return getQSettings().fileName().toStdString();
    }

    void saveChangedSettings()
    {
        if ( m_changedSettings.empty() )
        {
            return;
        }

        for ( const auto setting : m_changedSettings )
        {
            setting->saveValue();
        }

        m_changedSettings.clear();
    }

    void saveAllSettings()
    {
        for ( auto& setting : m_boolSettings )
        {
            setting.saveValue();
        }
    }

    template <typename ReturnType, typename Setting>
    [[nodiscard]] ReturnType getSetting( const Setting setting ) const noexcept
    {
        const auto index = static_cast<std::size_t>( setting );

        if constexpr ( std::is_same<Setting, BoolSetting>::value )
        {
            return m_boolSettings[index].value();
        }
        else if constexpr ( std::is_same<Setting, DoubleSetting>::value )
        {
            return m_doubleSettings[index].value();
        }
        else if constexpr ( std::is_same<Setting, IntSetting>::value )
        {
            return m_intSettings[index].value();
        }
        else if constexpr ( std::is_same<Setting, StringSetting>::value )
        {
            return m_stringSettings[index].value();
        }
    }

    template <typename Setting, typename Type>
    void setSetting( const Setting setting, const Type value ) noexcept
    {
        const auto index = static_cast<std::size_t>( setting );

        if constexpr ( std::is_same<Setting, BoolSetting>::value )
        {
            auto& s = m_boolSettings[index];
            s.setValue( value );

            m_changedSettings.push_back( &s );
        }
        else if constexpr ( std::is_same<Setting, DoubleSetting>::value )
        {
            auto& s = m_doubleSettings[index];
            s.setValue( value );

            m_changedSettings.push_back( &s );
        }
        else if constexpr ( std::is_same<Setting, IntSetting>::value )
        {
            auto& s = m_intSettings[index];
            s.setValue( value );

            m_changedSettings.push_back( &s );
        }
        else if constexpr ( std::is_same<Setting, StringSetting>::value )
        {
            auto& s = m_stringSettings[index];
            s.setValue( value );

            m_changedSettings.push_back( &s );
        }
    }

private:
    std::vector<SettingValue*> m_changedSettings{};

    constexpr static auto boolSettingSize
        = static_cast<int>( BoolSetting::LAST_ENUMERATOR ) + 1;
    std::array<BoolSettingValue, boolSettingSize> m_boolSettings{
        BoolSettingValue{ BoolSetting::PLAYSPACE_lockXToggle,
                          SettingCategory::Playspace,
                          QtInfo{ "lockXToggle" },
                          false },
        BoolSettingValue{ BoolSetting::PLAYSPACE_lockYToggle,
                          SettingCategory::Playspace,
                          QtInfo{ "lockYToggle" },
                          false },
        BoolSettingValue{ BoolSetting::PLAYSPACE_lockZToggle,
                          SettingCategory::Playspace,
                          QtInfo{ "lockZToggle" },
                          false },
        BoolSettingValue{ BoolSetting::PLAYSPACE_momentumSave,
                          SettingCategory::Playspace,
                          QtInfo{ "momentumSave" },
                          false },
        BoolSettingValue{ BoolSetting::PLAYSPACE_turnBindLeft,
                          SettingCategory::Playspace,
                          QtInfo{ "turnBindLeft" },
                          false },
        BoolSettingValue{ BoolSetting::PLAYSPACE_turnBindRight,
                          SettingCategory::Playspace,
                          QtInfo{ "turnBindRight" },
                          false },
        BoolSettingValue{ BoolSetting::PLAYSPACE_turnBounds,
                          SettingCategory::Playspace,
                          QtInfo{ "turnBounds" },
                          false },
        BoolSettingValue{ BoolSetting::PLAYSPACE_moveShortcutLeft,
                          SettingCategory::Playspace,
                          QtInfo{ "moveShortcutLeft" },
                          false },
        BoolSettingValue{ BoolSetting::PLAYSPACE_moveShortcutRight,
                          SettingCategory::Playspace,
                          QtInfo{ "moveShortcutRight" },
                          false },
        BoolSettingValue{ BoolSetting::PLAYSPACE_dragBounds,
                          SettingCategory::Playspace,
                          QtInfo{ "dragBounds" },
                          false },
        BoolSettingValue{ BoolSetting::PLAYSPACE_allowExternalEdits,
                          SettingCategory::Playspace,
                          QtInfo{ "allowExternalEdits" },
                          false },
        BoolSettingValue{ BoolSetting::PLAYSPACE_oldStyleMotion,
                          SettingCategory::Playspace,
                          QtInfo{ "oldStyleMotion" },
                          false },
        BoolSettingValue{ BoolSetting::PLAYSPACE_universeCenteredRotation,
                          SettingCategory::Playspace,
                          QtInfo{ "universeCenteredRotation" },
                          false },
        BoolSettingValue{ BoolSetting::PLAYSPACE_enableSeatedMotion,
                          SettingCategory::Playspace,
                          QtInfo{ "enableSeatedMotion" },
                          false },
        BoolSettingValue{ BoolSetting::PLAYSPACE_adjustChaperone,
                          SettingCategory::Playspace,
                          QtInfo{ "adjustChaperone" },
                          true },
        BoolSettingValue{ BoolSetting::PLAYSPACE_showLogMatricesButton,
                          SettingCategory::Playspace,
                          QtInfo{ "showLogMatricesButton" },
                          false },
        BoolSettingValue{ BoolSetting::PLAYSPACE_simpleRecenter,
                          SettingCategory::Playspace,
                          QtInfo{ "simpleRecenter" },
                          false },

        BoolSettingValue{ BoolSetting::APPLICATION_disableVersionCheck,
                          SettingCategory::Application,
                          QtInfo{ "disableVersionCheck" },
                          false },
        BoolSettingValue{ BoolSetting::APPLICATION_previousShutdownSafe,
                          SettingCategory::Application,
                          QtInfo{ "previousShutdownSafe" },
                          true },
        BoolSettingValue{ BoolSetting::APPLICATION_vsyncDisabled,
                          SettingCategory::Application,
                          QtInfo{ "vsyncDisabled" },
                          false },
        BoolSettingValue{ BoolSetting::APPLICATION_crashRecoveryDisabled,
                          SettingCategory::Application,
                          QtInfo{ "crashRecoveryDisabled" },
                          false },
        BoolSettingValue{ BoolSetting::APPLICATION_enableDebug,
                          SettingCategory::Application,
                          QtInfo{ "enableDebug" },
                          false },

        BoolSettingValue{ BoolSetting::AUDIO_micProximitySensorCanMute,
                          SettingCategory::Audio,
                          QtInfo{ "micProximitySensorCanMute" },
                          false },

        BoolSettingValue{ BoolSetting::VIDEO_brightnessEnabled,
                          SettingCategory::Video,
                          QtInfo{ "brightnessEnabled" },
                          false },
        BoolSettingValue{ BoolSetting::VIDEO_isOverlayMethodActive,
                          SettingCategory::Video,
                          QtInfo{ "isOverlayMethodActive" },
                          false },
        BoolSettingValue{ BoolSetting::VIDEO_colorOverlayEnabled,
                          SettingCategory::Video,
                          QtInfo{ "colorOverlayEnabled" },
                          false },

        BoolSettingValue{ BoolSetting::CHAPERONE_disableChaperone,
                          SettingCategory::Chaperone,
                          QtInfo{ "disableChaperone" },
                          false },
    };

    constexpr static auto doubleSettingSize
        = static_cast<int>( DoubleSetting::LAST_ENUMERATOR ) + 1;
    std::array<DoubleSettingValue, doubleSettingSize> m_doubleSettings{
        DoubleSettingValue{ DoubleSetting::PLAYSPACE_heightToggleOffset,
                            SettingCategory::Playspace,
                            QtInfo{ "heightToggleOffset" },
                            -1.0 },
        DoubleSettingValue{ DoubleSetting::PLAYSPACE_gravityStrength,
                            SettingCategory::Playspace,
                            QtInfo{ "gravityStrength" },
                            9.8 },
        DoubleSettingValue{ DoubleSetting::PLAYSPACE_flingStrength,
                            SettingCategory::Playspace,
                            QtInfo{ "flingStrength" },
                            1.0 },
        DoubleSettingValue{ DoubleSetting::UTILITY_desktopWidth,
                            SettingCategory::Utility,
                            QtInfo{ "desktopWidth" },
                            4.0 },

        DoubleSettingValue{ DoubleSetting::VIDEO_brightnessOpacityValue,
                            SettingCategory::Video,
                            QtInfo{ "brightnessOpacityValue" },
                            0.0 },
        DoubleSettingValue{ DoubleSetting::VIDEO_brightnessValue,
                            SettingCategory::Video,
                            QtInfo{ "brightnessValue" },
                            1.0 },
        DoubleSettingValue{ DoubleSetting::VIDEO_colorOverlayOpacity,
                            SettingCategory::Video,
                            QtInfo{ "colorOverlayOpacity" },
                            0.0 },
        DoubleSettingValue{ DoubleSetting::VIDEO_colorRed,
                            SettingCategory::Video,
                            QtInfo{ "colorRed" },
                            1.0 },
        DoubleSettingValue{ DoubleSetting::VIDEO_colorGreen,
                            SettingCategory::Video,
                            QtInfo{ "colorGreen" },
                            1.0 },
        DoubleSettingValue{ DoubleSetting::VIDEO_colorBlue,
                            SettingCategory::Video,
                            QtInfo{ "colorBlue" },
                            1.0 },
        DoubleSettingValue{ DoubleSetting::VIDEO_colorRedNew,
                            SettingCategory::Video,
                            QtInfo{ "colorRedNew" },
                            1.0 },
        DoubleSettingValue{ DoubleSetting::VIDEO_colorGreenNew,
                            SettingCategory::Video,
                            QtInfo{ "colorGreenNew" },
                            1.0 },
        DoubleSettingValue{ DoubleSetting::VIDEO_colorBlueNew,
                            SettingCategory::Video,
                            QtInfo{ "colorBlueNew" },
                            1.0 },

        DoubleSettingValue{ DoubleSetting::CHAPERONE_switchToBeginnerDistance,
                            SettingCategory::Video,
                            QtInfo{ "switchToBeginnerDistance" },
                            0.5 },
        DoubleSettingValue{ DoubleSetting::CHAPERONE_hapticFeedbackDistance,
                            SettingCategory::Video,
                            QtInfo{ "hapticFeedbackDistance" },
                            0.5 },
        DoubleSettingValue{ DoubleSetting::CHAPERONE_alarmSoundDistance,
                            SettingCategory::Video,
                            QtInfo{ "alarmSoundDistance" },
                            0.5 },
        DoubleSettingValue{ DoubleSetting::CHAPERONE_showDashboardDistance,
                            SettingCategory::Video,
                            QtInfo{ "showDashboardDistance" },
                            0.5 },
        DoubleSettingValue{ DoubleSetting::CHAPERONE_fadeDistanceRemembered,
                            SettingCategory::Chaperone,
                            QtInfo{ "fadeDistanceRemembered" },
                            0.5 },
    };

    constexpr static auto stringSettingsSize
        = static_cast<int>( StringSetting::LAST_ENUMERATOR ) + 1;
    constexpr static auto discordDefaultMuteKeybinding = "^>m";
    std::array<StringSettingValue, stringSettingsSize> m_stringSettings{
        StringSettingValue{ StringSetting::KEYBOARDSHORTCUT_keyboardOne,
                            SettingCategory::KeyboardShortcut,
                            QtInfo{ "keyboardOne" },
                            discordDefaultMuteKeybinding },
        StringSettingValue{ StringSetting::KEYBOARDSHORTCUT_keyboardTwo,
                            SettingCategory::KeyboardShortcut,
                            QtInfo{ "keyboardTwo" },
                            discordDefaultMuteKeybinding },
        StringSettingValue{ StringSetting::KEYBOARDSHORTCUT_keyboardThree,
                            SettingCategory::KeyboardShortcut,
                            QtInfo{ "keyboardThree" },
                            discordDefaultMuteKeybinding },
    };

    constexpr static auto intSettingsSize
        = static_cast<int>( IntSetting::LAST_ENUMERATOR ) + 1;
    std::array<IntSettingValue, intSettingsSize> m_intSettings{
        IntSettingValue{ IntSetting::PLAYSPACE_snapTurnAngle,
                         SettingCategory::Playspace,
                         QtInfo{ "snapTurnAngle" },
                         4500 },
        IntSettingValue{ IntSetting::PLAYSPACE_smoothTurnRate,
                         SettingCategory::Playspace,
                         QtInfo{ "smoothTurnRate" },
                         100 },
        IntSettingValue{ IntSetting::PLAYSPACE_dragComfortFactor,
                         SettingCategory::Playspace,
                         QtInfo{ "dragComfortFactor" },
                         0 },
        IntSettingValue{ IntSetting::PLAYSPACE_turnComfortFactor,
                         SettingCategory::Playspace,
                         QtInfo{ "turnComfortFactor" },
                         0 },

        IntSettingValue{ IntSetting::APPLICATION_debugState,
                         SettingCategory::Application,
                         QtInfo{ "debugState" },
                         0 },
        IntSettingValue{ IntSetting::APPLICATION_customTickRateMs,
                         SettingCategory::Application,
                         QtInfo{ "customTickRateMs" },
                         20 },

        IntSettingValue{ IntSetting::UTILITY_alarmHour,
                         SettingCategory::Utility,
                         QtInfo{ "alarmHour" },
                         0 },
        IntSettingValue{ IntSetting::UTILITY_alarmMinute,
                         SettingCategory::Utility,
                         QtInfo{ "alarmMinute" },
                         0 },

    };
};
} // namespace settings
