#pragma once
#include <string>

namespace settings
{
enum class BoolSetting
{
    PLAYSPACE_lockXToggle,
    PLAYSPACE_lockYToggle,
    PLAYSPACE_lockZToggle,
    PLAYSPACE_momentumSave,
    PLAYSPACE_turnBindLeft,
    PLAYSPACE_turnBindRight,
    PLAYSPACE_turnBounds,
    PLAYSPACE_moveShortcutLeft,
    PLAYSPACE_moveShortcutRight,
    PLAYSPACE_dragBounds,
    PLAYSPACE_allowExternalEdits,
    PLAYSPACE_oldStyleMotion,
    PLAYSPACE_universeCenteredRotation,
    PLAYSPACE_enableSeatedMotion,
    PLAYSPACE_adjustChaperone,

    APPLICATION_disableVersionCheck,
    APPLICATION_previousShutdownSafe,

    VIDEO_brightnessEnabled,
    VIDEO_isOverlayMethodActive,
    VIDEO_colorOverlayEnabled,

    CHAPERONE_disableChaperone,
    // LAST_ENUMERATOR must always be set to the last value
    LAST_ENUMERATOR = CHAPERONE_disableChaperone,
};

enum class DoubleSetting
{
    UTILITY_desktopWidth,

    VIDEO_brightnessOpacityValue,
    VIDEO_brightnessValue,
    VIDEO_colorOverlayOpacity,
    VIDEO_colorRed,
    VIDEO_colorGreen,
    VIDEO_colorBlue,
    VIDEO_colorRedNew,
    VIDEO_colorGreenNew,
    VIDEO_colorBlueNew,

    CHAPERONE_switchToBeginnerDistance,
    CHAPERONE_hapticFeedbackDistance,
    CHAPERONE_alarmSoundDistance,
    CHAPERONE_showDashboardDistance,
    CHAPERONE_fadeDistanceRemembered,
    // LAST_ENUMERATOR must always be set to the last value
    LAST_ENUMERATOR = CHAPERONE_fadeDistanceRemembered,
};

enum class StringSetting
{
    KEYBOARDSHORTCUT_keyboardOne,
    KEYBOARDSHORTCUT_keyboardTwo,
    KEYBOARDSHORTCUT_keyboardThree,
    // LAST_ENUMERATOR must always be set to the last value
    LAST_ENUMERATOR = KEYBOARDSHORTCUT_keyboardThree,
};

enum class IntSetting
{
    UTILITY_alarmHour,
    UTILITY_alarmMinute,
    // LAST_ENUMERATOR must always be set to the last value
    LAST_ENUMERATOR = UTILITY_alarmMinute,
};

std::string initializeAndGetSettingsPath();

void saveChangedSettings();

void saveAllSettings();

[[nodiscard]] bool getSetting( const BoolSetting setting );
void setSetting( const BoolSetting setting, const bool value );

[[nodiscard]] double getSetting( const DoubleSetting setting );
void setSetting( const DoubleSetting setting, const double value );

[[nodiscard]] int getSetting( const IntSetting setting );
void setSetting( const IntSetting setting, const int value );

[[nodiscard]] std::string getSetting( const StringSetting setting );
void setSetting( const StringSetting setting, const std::string value );

} // namespace settings
