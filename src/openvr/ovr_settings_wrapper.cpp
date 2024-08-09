#include "ovr_settings_wrapper.h"

#include <QtDebug>
#include <QtLogging>
#include <cmath>
#include <utility>

namespace ovr_settings_wrapper
{
// cludgey solution, long term solution we should apply defaults that make sense
// (most likely false)
bool unsetSettingErrorEnabled = true;

SettingsError handleErrors( std::string settingsKey,
                            vr::EVRSettingsError error,
                            std::string customErrorMsg )
{
    if ( error != vr::VRSettingsError_None )
    {
        if ( !unsetSettingErrorEnabled
             && error
                    == vr::EVRSettingsError::
                        VRSettingsError_UnsetSettingHasNoDefault )
        {
            return SettingsError::UndefinedError;
        }
        qCritical() << "Could not access \"" << settingsKey << "\" setting: "
                    << vr::VRSettings()->GetSettingsErrorNameFromEnum( error )
                    << " " << customErrorMsg;
        if ( error
             == vr::EVRSettingsError::VRSettingsError_UnsetSettingHasNoDefault )
        {
            unsetSettingErrorEnabled = false;
        }

        return SettingsError::UndefinedError;
    }
    return SettingsError::NoError;
}

std::pair<SettingsError, bool> getBool( std::string section,
                                        std::string settingsKey,
                                        std::string customErrorMsg )

{
    bool value = false;
    vr::EVRSettingsError error = {};
    value = vr::VRSettings()->GetBool(
        section.c_str(), settingsKey.c_str(), &error );
    SettingsError const err
        = handleErrors( settingsKey, error, customErrorMsg );
    return std::make_pair( err, value );
}

std::pair<SettingsError, int> getInt32( std::string section,
                                        std::string settingsKey,
                                        std::string customErrorMsg )

{
    int value = 0;
    vr::EVRSettingsError error = {};
    value = static_cast<int>( vr::VRSettings()->GetInt32(
        section.c_str(), settingsKey.c_str(), &error ) );
    SettingsError const err
        = handleErrors( settingsKey, error, customErrorMsg );
    return std::make_pair( err, value );
}

std::pair<SettingsError, float> getFloat( std::string section,
                                          std::string settingsKey,
                                          std::string customErrorMsg )

{
    float value = NAN;
    vr::EVRSettingsError error = {};
    value = vr::VRSettings()->GetFloat(
        section.c_str(), settingsKey.c_str(), &error );
    handleErrors( settingsKey, error, customErrorMsg );
    SettingsError const err
        = handleErrors( settingsKey, error, customErrorMsg );
    return std::make_pair( err, value );
}

std::pair<SettingsError, std::string> getString( std::string section,
                                                 std::string settingsKey,
                                                 std::string customErrorMsg )

{
    vr::EVRSettingsError error = {};
    // This appears to be correct value as set in CVRSettingHelper as of
    // ovr 1.11.11
    const uint32_t bufferMax = 4096;
    std::array<char, bufferMax> cStringOut;

    vr::VRSettings()->GetString( section.c_str(),
                                 settingsKey.c_str(),
                                 cStringOut.data(),
                                 bufferMax,
                                 &error );
    handleErrors( settingsKey, error, customErrorMsg );
    std::string const value = cStringOut.data();
    SettingsError const err
        = handleErrors( settingsKey, error, customErrorMsg );
    return std::make_pair( err, value );
}

// Setters

SettingsError setBool( std::string section,
                       std::string settingsKey,
                       bool value,
                       std::string customErrorMsg )
{
    vr::EVRSettingsError error = {};
    vr::VRSettings()->SetBool(
        section.c_str(), settingsKey.c_str(), value, &error );
    return handleErrors( settingsKey, error, customErrorMsg );
}

SettingsError setInt32( std::string section,
                        std::string settingsKey,
                        int value,
                        std::string customErrorMsg )
{
    vr::EVRSettingsError error = {};
    vr::VRSettings()->SetInt32( section.c_str(),
                                settingsKey.c_str(),
                                static_cast<int32_t>( value ),
                                &error );
    return handleErrors( settingsKey, error, customErrorMsg );
}

SettingsError setFloat( std::string section,
                        std::string settingsKey,
                        float value,
                        std::string customErrorMsg )
{
    vr::EVRSettingsError error = {};
    vr::VRSettings()->SetFloat(
        section.c_str(), settingsKey.c_str(), value, &error );
    return handleErrors( settingsKey, error, customErrorMsg );
}

SettingsError setString( std::string section,
                         std::string settingsKey,
                         char* value,
                         std::string customErrorMsg )
{
    vr::EVRSettingsError error = {};
    vr::VRSettings()->SetString(
        section.c_str(), settingsKey.c_str(), value, &error );
    return handleErrors( settingsKey, error, customErrorMsg );
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
SettingsError removeSection( std::string section, std::string customErrorMsg )
{
    vr::EVRSettingsError error = {};
    vr::VRSettings()->RemoveSection( section.c_str(), &error );
    return handleErrors( "section", error, customErrorMsg );
}

SettingsError removeKeyInSection( std::string section,
                                  std::string settingsKey,
                                  std::string customErrorMsg )
{
    vr::EVRSettingsError error = {};
    vr::VRSettings()->RemoveKeyInSection(
        section.c_str(), settingsKey.c_str(), &error );
    return handleErrors( settingsKey, error, customErrorMsg );
}

void resetAllSettings()
{
    // these should cover all sections we touch and should be added to if we
    // touch more or they change
    removeSection( vr::k_pch_SteamVR_Section, "While Resetting All" );
    removeSection( vr::k_pch_Notifications_Section, "While Resetting All" );
    removeSection( vr::k_pch_CollisionBounds_Section, "While Resetting All" );
    removeSection( vr::k_pch_Camera_Section, "While Resetting All" );
    removeSection( vr::k_pch_audio_Section, "While Resetting All" );
}

} // namespace ovr_settings_wrapper
