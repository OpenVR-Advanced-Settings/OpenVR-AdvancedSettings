#pragma once
#include <assert.h>
#include <array>
#include <vector>
#include <easylogging++.h>
#include "../settings.h"
#include "setting_value.h"
#include "../../utils/setup.h"
#include "specific_setting_value.h"
#include "../../tabcontrollers/MoveCenterTabController.h"

namespace settings
{
template <typename Value> std::string valueToString( Value value )
{
    using std::is_same;
    if constexpr ( is_same<bool, Value>::value )
    {
        return value ? "true" : "false";
    }
    else if constexpr ( is_same<double, Value>::value
                        || is_same<int, Value>::value )
    {
        return std::to_string( value );
    }
    if constexpr ( is_same<std::string, Value>::value )
    {
        return value;
    }
}

template <int ArraySize, typename Value>
[[nodiscard]] std::string
    returnSettingsAndValues( std::array<Value, ArraySize> v )
{
    std::string s = "default";

    for ( const auto& setting : v )
    {
        s += setting.qtInfo().settingName + ": '"
             + valueToString( setting.value() ) + "' | ";
    }

    return s;
}
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

    std::string getSettingsAndValues() const noexcept
    {
        std::string s;
        s += returnSettingsAndValues<boolSettingSize>( m_boolSettings );

        s += returnSettingsAndValues<doubleSettingSize>( m_doubleSettings );

        s += returnSettingsAndValues<stringSettingsSize>( m_stringSettings );

        s += returnSettingsAndValues<intSettingsSize>( m_intSettings );

        return s;
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
        for ( auto& setting : m_doubleSettings )
        {
            setting.saveValue();
        }
        for ( auto& setting : m_stringSettings )
        {
            setting.saveValue();
        }
        for ( auto& setting : m_intSettings )
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
        // TODO Replace Back to Adjust Chaperone When Breaking Change is done.
        // Needed New Setting defaulted to off w/ SVR 1.13.x release
        BoolSettingValue{ BoolSetting::PLAYSPACE_adjustChaperone2,
                          SettingCategory::Playspace,
                          QtInfo{ "adjustChaperone2" },
                          false },
        BoolSettingValue{ BoolSetting::PLAYSPACE_enableUncalMotion,
                          SettingCategory::Playspace,
                          QtInfo{ "enableUncalMotion" },
                          false },
        BoolSettingValue{ BoolSetting::PLAYSPACE_adjustChaperone3,
                          SettingCategory::Playspace,
                          QtInfo{ "adjustChaperone3" },
                          true },
        BoolSettingValue{ BoolSetting::PLAYSPACE_adjustChaperone4,
                          SettingCategory::Playspace,
                          QtInfo{ "adjustChaperone4" },
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
        BoolSettingValue{ BoolSetting::APPLICATION_enableExclusiveInput,
                          SettingCategory::Application,
                          QtInfo{ "enableExclusiveInput" },
                          false },
        BoolSettingValue{ BoolSetting::APPLICATION_crashRecoveryDisabled2,
                          SettingCategory::Application,
                          QtInfo{ "crashRecoveryDisabled2" },
                          true },
        BoolSettingValue{ BoolSetting::APPLICATION_openXRWorkAround,
                          SettingCategory::Application,
                          QtInfo{ "openXRWorkAround" },
                          false },
        BoolSettingValue{ BoolSetting::APPLICATION_autoApplyChaperone,
                          SettingCategory::Application,
                          QtInfo{ "autoApplyChaperoneToggle" },
                          false },
        BoolSettingValue{ BoolSetting::APPLICATION_desktopModeToggle,
                          SettingCategory::Application,
                          QtInfo{ "desktopModeToggle" },
                          false },

        BoolSettingValue{ BoolSetting::AUDIO_pttEnabled,
                          SettingCategory::Audio,
                          QtInfo{ "pttEnabled" },
                          false },
        BoolSettingValue{ BoolSetting::AUDIO_pttShowNotification,
                          SettingCategory::Audio,
                          QtInfo{ "pttShowNotification" },
                          false },
        BoolSettingValue{ BoolSetting::AUDIO_micProximitySensorCanMute,
                          SettingCategory::Audio,
                          QtInfo{ "micProximitySensorCanMute" },
                          false },
        BoolSettingValue{ BoolSetting::AUDIO_micReversePtt,
                          SettingCategory::Audio,
                          QtInfo{ "micReversePtt" },
                          false },

        BoolSettingValue{ BoolSetting::UTILITY_alarmEnabled,
                          SettingCategory::Utility,
                          QtInfo{ "alarmEnabled" },
                          false },
        BoolSettingValue{ BoolSetting::UTILITY_alarmIsModal,
                          SettingCategory::Utility,
                          QtInfo{ "alarmIsModal" },
                          true },
        BoolSettingValue{ BoolSetting::UTILITY_vrcDebug,
                          SettingCategory::Utility,
                          QtInfo{ "vrcDebug" },
                          false },
        BoolSettingValue{ BoolSetting::UTILITY_trackerOverlayEnabled,
                          SettingCategory::Utility,
                          QtInfo{ "trackerOverlayEnabled" },
                          true },

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

        BoolSettingValue{
            BoolSetting::CHAPERONE_chaperoneSwitchToBeginnerEnabled,
            SettingCategory::Chaperone,
            QtInfo{ "chaperoneSwitchToBeginnerEnabled" },
            false },
        BoolSettingValue{ BoolSetting::CHAPERONE_chaperoneHapticFeedbackEnabled,
                          SettingCategory::Chaperone,
                          QtInfo{ "chaperoneHapticFeedbackEnabled" },
                          false },
        BoolSettingValue{ BoolSetting::CHAPERONE_chaperoneAlarmSoundEnabled,
                          SettingCategory::Chaperone,
                          QtInfo{ "chaperoneAlarmSoundEnabled" },
                          false },
        BoolSettingValue{ BoolSetting::CHAPERONE_chaperoneAlarmSoundLooping,
                          SettingCategory::Chaperone,
                          QtInfo{ "chaperoneAlarmSoundLooping" },
                          true },
        BoolSettingValue{
            BoolSetting::CHAPERONE_chaperoneAlarmSoundAdjustVolume,
            SettingCategory::Chaperone,
            QtInfo{ "chaperoneAlarmSoundAdjustVolume" },
            false },
        BoolSettingValue{ BoolSetting::CHAPERONE_chaperoneShowDashboardEnabled,
                          SettingCategory::Chaperone,
                          QtInfo{ "chaperoneShowDashboardEnabled" },
                          false },
        BoolSettingValue{ BoolSetting::CHAPERONE_disableChaperone,
                          SettingCategory::Chaperone,
                          QtInfo{ "disableChaperone" },
                          false },
        BoolSettingValue{ BoolSetting::CHAPERONE_centerMarkerNew,
                          SettingCategory::Chaperone,
                          QtInfo{ "centerMarkerNew" },
                          false },

        BoolSettingValue{ BoolSetting::ROTATION_autoturnEnabled,
                          SettingCategory::Rotation,
                          QtInfo{ "autoturnEnabled" },
                          false },
        BoolSettingValue{ BoolSetting::ROTATION_autoturnUseCornerAngle,
                          SettingCategory::Rotation,
                          QtInfo{ "autoturnUseCornerAngle" },
                          true },
        BoolSettingValue{ BoolSetting::ROTATION_autoturnVestibularMotionEnabled,
                          SettingCategory::Rotation,
                          QtInfo{ "autoturnVestibularMotionEnabled" },
                          false },
        BoolSettingValue{ BoolSetting::ROTATION_autoturnViewRatchettingEnabled,
                          SettingCategory::Rotation,
                          QtInfo{ "autoturnViewRatchettingEnabled" },
                          false },
        BoolSettingValue{ BoolSetting::ROTATION_autoturnShowNotification,
                          SettingCategory::Rotation,
                          QtInfo{ "autoturnShowNotification" },
                          true },
        BoolSettingValue{ BoolSetting::STEAMVR_perappBindEnabled,
                          SettingCategory::SteamVR,
                          QtInfo{ "perappBindEnabled" },
                          false }
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
        DoubleSettingValue{ DoubleSetting::PLAYSPACE_dragMult,
                            SettingCategory::Playspace,
                            QtInfo{ "dragMult" },
                            1.0 },

        DoubleSettingValue{ DoubleSetting::APPLICATION_appVolume,
                            SettingCategory::Application,
                            QtInfo{ "appVolume" },
                            0.7 },

        DoubleSettingValue{ DoubleSetting::VIDEO_brightnessOpacityValue,
                            SettingCategory::Video,
                            QtInfo{ "brightnessOpacityValue" },
                            1.0 },
        DoubleSettingValue{ DoubleSetting::VIDEO_colorOverlayOpacity,
                            SettingCategory::Video,
                            QtInfo{ "colorOverlayOpacity" },
                            0.0 },
        DoubleSettingValue{ DoubleSetting::VIDEO_colorRed,
                            SettingCategory::Video,
                            QtInfo{ "colorRedNew" },
                            1.0 },
        DoubleSettingValue{ DoubleSetting::VIDEO_colorGreen,
                            SettingCategory::Video,
                            QtInfo{ "colorGreenNew" },
                            1.0 },
        DoubleSettingValue{ DoubleSetting::VIDEO_colorBlue,
                            SettingCategory::Video,
                            QtInfo{ "colorBlueNew" },
                            1.0 },

        // TODO BUG [breaking change] should be in Chaperone Settings Not Video
        // fix @ breaking change
        DoubleSettingValue{ DoubleSetting::CHAPERONE_switchToBeginnerDistance,
                            SettingCategory::Video,
                            QtInfo{ "chaperoneSwitchToBeginnerDistance" },
                            0.5 },
        // TODO BUG [breaking change] should be in Chaperone Settings Not Video
        // fix @ breaking change
        DoubleSettingValue{ DoubleSetting::CHAPERONE_hapticFeedbackDistance,
                            SettingCategory::Video,
                            QtInfo{ "chaperoneHapticFeedbackDistance" },
                            0.5 },
        // TODO BUG [breaking change] should be in Chaperone Settings Not Video
        // fix @ breaking change
        DoubleSettingValue{ DoubleSetting::CHAPERONE_alarmSoundDistance,
                            SettingCategory::Video,
                            QtInfo{ "chaperoneAlarmSoundDistance" },
                            0.5 },
        // TODO BUG [breaking change] should be in Chaperone Settings Not Video
        // fix @ breaking change
        DoubleSettingValue{ DoubleSetting::CHAPERONE_showDashboardDistance,
                            SettingCategory::Video,
                            QtInfo{ "chaperoneShowDashboardDistance" },
                            0.5 },
        DoubleSettingValue{ DoubleSetting::CHAPERONE_fadeDistanceRemembered,
                            SettingCategory::Chaperone,
                            QtInfo{ "fadeDistanceRemembered" },
                            0.5 },
        DoubleSettingValue{ DoubleSetting::CHAPERONE_dimHeight,
                            SettingCategory::Chaperone,
                            QtInfo{ "dimHeight" },
                            0.0 },
        DoubleSettingValue{ DoubleSetting::ROTATION_activationDistance,
                            SettingCategory::Rotation,
                            QtInfo{ "activationDistance" },
                            0.4 },
        DoubleSettingValue{ DoubleSetting::ROTATION_deactivateDistance,
                            SettingCategory::Rotation,
                            QtInfo{ "deactivateDistance" },
                            0.15 },
        DoubleSettingValue{ DoubleSetting::ROTATION_cordDetanglingAngle,
                            SettingCategory::Rotation,
                            QtInfo{ "cordDetanglingAngle" },
                            1500 * advsettings::k_centidegreesToRadians },
        DoubleSettingValue{ DoubleSetting::ROTATION_autoturnMinCordTangle,
                            SettingCategory::Rotation,
                            QtInfo{ "autoturnMinCordTangle" },
                            2 * M_PI },
        DoubleSettingValue{
            DoubleSetting::ROTATION_autoturnVestibularMotionRadius,
            SettingCategory::Rotation,
            QtInfo{ "autoturnVestibularMotionRadius" },
            22.0 },
        DoubleSettingValue{
            DoubleSetting::ROTATION_autoturnViewRatchettingPercent,
            SettingCategory::Rotation,
            QtInfo{ "autoturnViewRatchettingPercent" },
            0.05 },
    };

    constexpr static auto stringSettingsSize
        = static_cast<int>( StringSetting::LAST_ENUMERATOR ) + 1;
    constexpr static auto discordDefaultMuteKeybinding = "^>m";
    constexpr static auto pressDefault = "F9";
    constexpr static auto nameDefault = "«none»";
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
        StringSettingValue{ StringSetting::KEYBOARDSHORTCUT_keyPressMisc,
                            SettingCategory::KeyboardShortcut,
                            QtInfo{ "keyPressmisc" },
                            pressDefault },
        StringSettingValue{ StringSetting::KEYBOARDSHORTCUT_keyPressSystem,
                            SettingCategory::KeyboardShortcut,
                            QtInfo{ "keyPressSystem" },
                            pressDefault },
        StringSettingValue{ StringSetting::APPLICATION_autoApplyChaperoneName,
                            SettingCategory::Application,
                            QtInfo{ "autoApplyChaperoneName" },
                            nameDefault },
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
        IntSettingValue{ IntSetting::PLAYSPACE_frictionPercent,
                         SettingCategory::Playspace,
                         QtInfo{ "frictionPercent" },
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
        IntSettingValue{ IntSetting::UTILITY_alarmSecond,
                         SettingCategory::Utility,
                         QtInfo{ "alarmSecond" },
                         0 },
        IntSettingValue{ IntSetting::ROTATION_autoturnLinearTurnSpeed,
                         SettingCategory::Rotation,
                         QtInfo{ "autoturnLinearTurnSpeed" },
                         45000 },
        IntSettingValue{ IntSetting::ROTATION_autoturnMode,
                         SettingCategory::Rotation,
                         QtInfo{ "autoturnMode" },
                         1 },

    };
};
} // namespace settings
