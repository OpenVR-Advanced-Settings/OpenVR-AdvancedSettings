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
    PLAYSPACE_showLogMatricesButton,
    PLAYSPACE_simpleRecenter,
    PLAYSPACE_adjustChaperone2,
    PLAYSPACE_enableUncalMotion,
    PLAYSPACE_adjustChaperone3,
    PLAYSPACE_adjustChaperone4,

    APPLICATION_disableVersionCheck,
    APPLICATION_previousShutdownSafe,
    APPLICATION_vsyncDisabled,
    APPLICATION_crashRecoveryDisabled,
    APPLICATION_enableDebug,
    APPLICATION_enableExclusiveInput,
    APPLICATION_crashRecoveryDisabled2,
    APPLICATION_openXRWorkAround,
    APPLICATION_autoApplyChaperone,
    APPLICATION_desktopModeToggle,

    AUDIO_pttEnabled,
    AUDIO_pttShowNotification,
    AUDIO_micProximitySensorCanMute,
    AUDIO_micReversePtt,

    UTILITY_alarmEnabled,
    UTILITY_alarmIsModal,
    UTILITY_vrcDebug,
    UTILITY_trackerOverlayEnabled,

    VIDEO_brightnessEnabled,
    VIDEO_isOverlayMethodActive,
    VIDEO_colorOverlayEnabled,

    CHAPERONE_chaperoneSwitchToBeginnerEnabled,
    CHAPERONE_chaperoneHapticFeedbackEnabled,
    CHAPERONE_chaperoneAlarmSoundEnabled,
    CHAPERONE_chaperoneAlarmSoundLooping,
    CHAPERONE_chaperoneAlarmSoundAdjustVolume,
    CHAPERONE_chaperoneShowDashboardEnabled,
    CHAPERONE_disableChaperone,
    CHAPERONE_centerMarkerNew,

    ROTATION_autoturnEnabled,
    ROTATION_autoturnUseCornerAngle,
    ROTATION_autoturnVestibularMotionEnabled,
    ROTATION_autoturnViewRatchettingEnabled,
    ROTATION_autoturnShowNotification,
    // LAST_ENUMERATOR must always be set to the last value
    STEAMVR_perappBindEnabled,
    LAST_ENUMERATOR = STEAMVR_perappBindEnabled,
};

enum class DoubleSetting
{
    PLAYSPACE_heightToggleOffset,
    PLAYSPACE_gravityStrength,
    PLAYSPACE_flingStrength,
    PLAYSPACE_dragMult,

    APPLICATION_appVolume,

    VIDEO_brightnessOpacityValue,
    VIDEO_colorOverlayOpacity,
    VIDEO_colorRed,
    VIDEO_colorGreen,
    VIDEO_colorBlue,

    CHAPERONE_switchToBeginnerDistance,
    CHAPERONE_hapticFeedbackDistance,
    CHAPERONE_alarmSoundDistance,
    CHAPERONE_showDashboardDistance,
    CHAPERONE_fadeDistanceRemembered,
    CHAPERONE_dimHeight,

    ROTATION_activationDistance,
    ROTATION_deactivateDistance,
    ROTATION_cordDetanglingAngle,
    ROTATION_autoturnMinCordTangle,
    ROTATION_autoturnVestibularMotionRadius,
    ROTATION_autoturnViewRatchettingPercent,
    // LAST_ENUMERATOR must always be set to the last value
    LAST_ENUMERATOR = ROTATION_autoturnViewRatchettingPercent,
};

enum class StringSetting
{
    KEYBOARDSHORTCUT_keyboardOne,
    KEYBOARDSHORTCUT_keyboardTwo,
    KEYBOARDSHORTCUT_keyboardThree,
    KEYBOARDSHORTCUT_keyPressMisc,
    KEYBOARDSHORTCUT_keyPressSystem,

    APPLICATION_autoApplyChaperoneName,

    // LAST_ENUMERATOR must always be set to the last value
    LAST_ENUMERATOR = APPLICATION_autoApplyChaperoneName,
};

enum class IntSetting
{
    PLAYSPACE_snapTurnAngle,
    PLAYSPACE_smoothTurnRate,
    PLAYSPACE_dragComfortFactor,
    PLAYSPACE_turnComfortFactor,
    PLAYSPACE_frictionPercent,

    APPLICATION_debugState,
    APPLICATION_customTickRateMs,

    UTILITY_alarmHour,
    UTILITY_alarmMinute,
    UTILITY_alarmSecond,

    ROTATION_autoturnLinearTurnSpeed,
    // LAST_ENUMERATOR must always be set to the last value
    ROTATION_autoturnMode,
    LAST_ENUMERATOR = ROTATION_autoturnMode,
};

std::string initializeAndGetSettingsPath();

std::string getSettingsAndValues();

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
