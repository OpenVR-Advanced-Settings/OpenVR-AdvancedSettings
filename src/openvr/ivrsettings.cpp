#include "ivrsettings.h"

namespace ivrsettings
{
settingsError handleErrors( logType errorType,
                            const char* pchSettingsKey,
                            vr::EVRSettingsError error,
                            std::string customErrorMsg )
{
    if ( error != vr::VRSettingsError_None )
    {
        switch ( errorType )
        {
        case logType::warn:
            LOG( WARNING ) << "Could not access \"" << pchSettingsKey
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  error )
                           << " " << customErrorMsg;
            break;
        case logType::err:
            LOG( ERROR ) << "Could not access \"" << pchSettingsKey
                         << "\" setting: "
                         << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                error )
                         << " " << customErrorMsg;
            break;
        case logType::bug:
            LOG( DEBUG ) << "Could not access \"" << pchSettingsKey
                         << "\" setting: "
                         << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                error )
                         << " " << customErrorMsg;
            break;
        }
        return settingsError::undefErr;
    }
    return settingsError::noErr;
}

std::pair<settingsError, bool> getBool( const char* pchSection,
                                        const char* pchSettingsKey,
                                        logType errorType,
                                        std::string customErrorMsg )

{
    bool value;
    vr::EVRSettingsError error;
    value = vr::VRSettings()->GetBool( pchSection, pchSettingsKey, &error );
    settingsError e
        = handleErrors( errorType, pchSettingsKey, error, customErrorMsg );
    std::pair<settingsError, bool> p( e, value );
    return p;
}

std::pair<settingsError, int> getInt32( const char* pchSection,
                                        const char* pchSettingsKey,
                                        logType errorType,
                                        std::string customErrorMsg )

{
    int value;
    vr::EVRSettingsError error;
    value = static_cast<int>(
        vr::VRSettings()->GetInt32( pchSection, pchSettingsKey, &error ) );
    settingsError e
        = handleErrors( errorType, pchSettingsKey, error, customErrorMsg );
    std::pair<settingsError, int> p( e, value );
    return p;
}

std::pair<settingsError, float> getFloat( const char* pchSection,
                                          const char* pchSettingsKey,
                                          logType errorType,
                                          std::string customErrorMsg )

{
    float value;
    vr::EVRSettingsError error;
    value = vr::VRSettings()->GetFloat( pchSection, pchSettingsKey, &error );
    handleErrors( errorType, pchSettingsKey, error, customErrorMsg );
    settingsError e
        = handleErrors( errorType, pchSettingsKey, error, customErrorMsg );
    std::pair<settingsError, float> p( e, value );
    return p;
}

// Setters

settingsError setBool( const char* pchSection,
                       const char* pchSettingsKey,
                       bool bValue,
                       logType errorType,
                       std::string customErrorMsg )
{
    vr::EVRSettingsError error;
    vr::VRSettings()->SetBool( pchSection, pchSettingsKey, bValue, &error );
    return handleErrors( errorType, pchSettingsKey, error, customErrorMsg );
}

settingsError setInt32( const char* pchSection,
                        const char* pchSettingsKey,
                        int nValue,
                        logType errorType,
                        std::string customErrorMsg )
{
    vr::EVRSettingsError error;
    vr::VRSettings()->SetInt32(
        pchSection, pchSettingsKey, static_cast<int32_t>( nValue ), &error );
    return handleErrors( errorType, pchSettingsKey, error, customErrorMsg );
}

settingsError setFloat( const char* pchSection,
                        const char* pchSettingsKey,
                        float flValue,
                        logType errorType,
                        std::string customErrorMsg )
{
    vr::EVRSettingsError error;
    vr::VRSettings()->SetFloat( pchSection, pchSettingsKey, flValue, &error );
    return handleErrors( errorType, pchSettingsKey, error, customErrorMsg );
}

settingsError removeSection( const char* pchSection,
                             logType errorType,
                             std::string customErrorMsg )
{
    vr::EVRSettingsError error;
    vr::VRSettings()->RemoveSection( pchSection, &error );
    return handleErrors( errorType, "Section", error, customErrorMsg );
}

settingsError removeKeyInSection( const char* pchSection,
                                  const char* pchSettingsKey,
                                  logType errorType,
                                  std::string customErrorMsg )
{
    vr::EVRSettingsError error;
    vr::VRSettings()->RemoveKeyInSection( pchSection, pchSettingsKey, &error );
    return handleErrors( errorType, pchSettingsKey, error, customErrorMsg );
}

} // namespace ivrsettings
