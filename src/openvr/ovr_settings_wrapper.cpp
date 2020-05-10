#include "ovr_settings_wrapper.h"

namespace ovr_settings_wrapper
{
SettingsError handleErrors( std::string settingsKey,
                            vr::EVRSettingsError error,
                            std::string customErrorMsg )
{
    if ( error != vr::VRSettingsError_None )
    {
        LOG( ERROR ) << "Could not access \"" << settingsKey << "\" setting: "
                     << vr::VRSettings()->GetSettingsErrorNameFromEnum( error )
                     << " " << customErrorMsg;

        return SettingsError::UndefinedError;
    }
    return SettingsError::NoError;
}

std::pair<SettingsError, bool> getBool( std::string section,
                                        std::string settingsKey,
                                        std::string customErrorMsg )

{
    bool value;
    vr::EVRSettingsError error;
    value = vr::VRSettings()->GetBool(
        section.c_str(), settingsKey.c_str(), &error );
    SettingsError e = handleErrors( settingsKey, error, customErrorMsg );
    std::pair<SettingsError, bool> p( e, value );
    return p;
}

std::pair<SettingsError, int> getInt32( std::string section,
                                        std::string settingsKey,
                                        std::string customErrorMsg )

{
    int value;
    vr::EVRSettingsError error;
    value = static_cast<int>( vr::VRSettings()->GetInt32(
        section.c_str(), settingsKey.c_str(), &error ) );
    SettingsError e = handleErrors( settingsKey, error, customErrorMsg );
    std::pair<SettingsError, int> p( e, value );
    return p;
}

std::pair<SettingsError, float> getFloat( std::string section,
                                          std::string settingsKey,
                                          std::string customErrorMsg )

{
    float value;
    vr::EVRSettingsError error;
    value = vr::VRSettings()->GetFloat(
        section.c_str(), settingsKey.c_str(), &error );
    handleErrors( settingsKey, error, customErrorMsg );
    SettingsError e = handleErrors( settingsKey, error, customErrorMsg );
    std::pair<SettingsError, float> p( e, value );
    return p;
}

// Setters

SettingsError setBool( std::string section,
                       std::string settingsKey,
                       bool bValue,
                       std::string customErrorMsg )
{
    vr::EVRSettingsError error;
    vr::VRSettings()->SetBool(
        section.c_str(), settingsKey.c_str(), bValue, &error );
    return handleErrors( settingsKey, error, customErrorMsg );
}

SettingsError setInt32( std::string section,
                        std::string settingsKey,
                        int nValue,
                        std::string customErrorMsg )
{
    vr::EVRSettingsError error;
    vr::VRSettings()->SetInt32( section.c_str(),
                                settingsKey.c_str(),
                                static_cast<int32_t>( nValue ),
                                &error );
    return handleErrors( settingsKey, error, customErrorMsg );
}

SettingsError setFloat( std::string section,
                        std::string settingsKey,
                        float flValue,
                        std::string customErrorMsg )
{
    vr::EVRSettingsError error;
    vr::VRSettings()->SetFloat(
        section.c_str(), settingsKey.c_str(), flValue, &error );
    return handleErrors( settingsKey, error, customErrorMsg );
}

SettingsError removeSection( std::string section, std::string customErrorMsg )
{
    vr::EVRSettingsError error;
    vr::VRSettings()->RemoveSection( section.c_str(), &error );
    return handleErrors( "section", error, customErrorMsg );
}

SettingsError removeKeyInSection( std::string section,
                                  std::string settingsKey,
                                  std::string customErrorMsg )
{
    vr::EVRSettingsError error;
    vr::VRSettings()->RemoveKeyInSection(
        section.c_str(), settingsKey.c_str(), &error );
    return handleErrors( settingsKey, error, customErrorMsg );
}

} // namespace ovr_settings_wrapper
