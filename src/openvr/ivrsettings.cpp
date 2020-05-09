#include "ivrsettings.h"

namespace ovr_settings_wrapper
{
SettingsError handleErrors( const char* pchSettingsKey,
                            vr::EVRSettingsError error,
                            std::string customErrorMsg )
{
    if ( error != vr::VRSettingsError_None )
    {
        LOG( ERROR ) << "Could not access \"" << pchSettingsKey
                     << "\" setting: "
                     << vr::VRSettings()->GetSettingsErrorNameFromEnum( error )
                     << " " << customErrorMsg;

        return SettingsError::UndefinedError;
    }
    return SettingsError::NoError;
}

std::pair<SettingsError, bool> getBool( const char* pchSection,
                                        const char* pchSettingsKey,
                                        std::string customErrorMsg )

{
    bool value;
    vr::EVRSettingsError error;
    value = vr::VRSettings()->GetBool( pchSection, pchSettingsKey, &error );
    SettingsError e = handleErrors( pchSettingsKey, error, customErrorMsg );
    std::pair<SettingsError, bool> p( e, value );
    return p;
}

std::pair<SettingsError, int> getInt32( const char* pchSection,
                                        const char* pchSettingsKey,
                                        std::string customErrorMsg )

{
    int value;
    vr::EVRSettingsError error;
    value = static_cast<int>(
        vr::VRSettings()->GetInt32( pchSection, pchSettingsKey, &error ) );
    SettingsError e = handleErrors( pchSettingsKey, error, customErrorMsg );
    std::pair<SettingsError, int> p( e, value );
    return p;
}

std::pair<SettingsError, float> getFloat( const char* pchSection,
                                          const char* pchSettingsKey,
                                          std::string customErrorMsg )

{
    float value;
    vr::EVRSettingsError error;
    value = vr::VRSettings()->GetFloat( pchSection, pchSettingsKey, &error );
    handleErrors( pchSettingsKey, error, customErrorMsg );
    SettingsError e = handleErrors( pchSettingsKey, error, customErrorMsg );
    std::pair<SettingsError, float> p( e, value );
    return p;
}

// Setters

SettingsError setBool( const char* pchSection,
                       const char* pchSettingsKey,
                       bool bValue,
                       std::string customErrorMsg )
{
    vr::EVRSettingsError error;
    vr::VRSettings()->SetBool( pchSection, pchSettingsKey, bValue, &error );
    return handleErrors( pchSettingsKey, error, customErrorMsg );
}

SettingsError setInt32( const char* pchSection,
                        const char* pchSettingsKey,
                        int nValue,
                        std::string customErrorMsg )
{
    vr::EVRSettingsError error;
    vr::VRSettings()->SetInt32(
        pchSection, pchSettingsKey, static_cast<int32_t>( nValue ), &error );
    return handleErrors( pchSettingsKey, error, customErrorMsg );
}

SettingsError setFloat( const char* pchSection,
                        const char* pchSettingsKey,
                        float flValue,
                        std::string customErrorMsg )
{
    vr::EVRSettingsError error;
    vr::VRSettings()->SetFloat( pchSection, pchSettingsKey, flValue, &error );
    return handleErrors( pchSettingsKey, error, customErrorMsg );
}

SettingsError removeSection( const char* pchSection,
                             std::string customErrorMsg )
{
    vr::EVRSettingsError error;
    vr::VRSettings()->RemoveSection( pchSection, &error );
    return handleErrors( "Section", error, customErrorMsg );
}

SettingsError removeKeyInSection( const char* pchSection,
                                  const char* pchSettingsKey,
                                  std::string customErrorMsg )
{
    vr::EVRSettingsError error;
    vr::VRSettings()->RemoveKeyInSection( pchSection, pchSettingsKey, &error );
    return handleErrors( pchSettingsKey, error, customErrorMsg );
}

} // namespace ovr_settings_wrapper
