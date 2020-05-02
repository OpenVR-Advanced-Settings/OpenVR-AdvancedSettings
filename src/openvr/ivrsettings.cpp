#include "ivrsettings.h"
#include <openvr.h>
#include <iostream>
#include <array>
#include <easylogging++.h>

namespace ivrsettings
{
void handleErrors( logType errorType,
                   const char* pchSettingsKey,
                   vr::EVRSettingsError error,
                   std::string customErrorMsg )
{
    if ( error != vr::VRSettingsError_None )
    {
        switch ( errorType )
        {
        case logType::warning:
            LOG( WARNING ) << "Could not read \"" << pchSettingsKey
                           << "\" setting: "
                           << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                  error )
                           << " " << customErrorMsg;
            break;
        case logType::err:
            LOG( ERROR ) << "Could not read \"" << pchSettingsKey
                         << "\" setting: "
                         << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                error )
                         << " " << customErrorMsg;
            break;
        case logType::debug:
            LOG( DEBUG ) << "Could not read \"" << pchSettingsKey
                         << "\" setting: "
                         << vr::VRSettings()->GetSettingsErrorNameFromEnum(
                                error )
                         << " " << customErrorMsg;
            break;
        }
    }
}

bool getBool( const char* pchSection,
              const char* pchSettingsKey,
              logType errorType,
              std::string customErrorMsg )

{
    bool value;
    vr::EVRSettingsError error;
    value = vr::VRSettings()->GetBool( pchSection, pchSettingsKey, &error );
    handleErrors( errorType, pchSettingsKey, error, customErrorMsg );
    return value;
}

int getInt32( const char* pchSection,
              const char* pchSettingsKey,
              logType errorType,
              std::string customErrorMsg )

{
    int value;
    vr::EVRSettingsError error;
    value = static_cast<int>(
        vr::VRSettings()->GetInt32( pchSection, pchSettingsKey, &error ) );
    handleErrors( errorType, pchSettingsKey, error, customErrorMsg );
    return value;
}

float getFloat( const char* pchSection,
                const char* pchSettingsKey,
                logType errorType,
                std::string customErrorMsg )

{
    float value;
    vr::EVRSettingsError error;
    value = vr::VRSettings()->GetFloat( pchSection, pchSettingsKey, &error );
    handleErrors( errorType, pchSettingsKey, error, customErrorMsg );
    return value;
}

// Setters

void setBool( const char* pchSection,
              const char* pchSettingsKey,
              bool bValue,
              logType errorType,
              std::string customErrorMsg )
{
    vr::EVRSettingsError error;
    vr::VRSettings()->SetBool( pchSection, pchSettingsKey, bValue, &error );
    handleErrors( errorType, pchSettingsKey, error, customErrorMsg );
}

void setInt32( const char* pchSection,
               const char* pchSettingsKey,
               int nValue,
               logType errorType,
               std::string customErrorMsg )
{
    vr::EVRSettingsError error;
    vr::VRSettings()->SetInt32(
        pchSection, pchSettingsKey, static_cast<int32_t>( nValue ), &error );
    handleErrors( errorType, pchSettingsKey, error, customErrorMsg );
}

void setFloat( const char* pchSection,
               const char* pchSettingsKey,
               float flValue,
               logType errorType,
               std::string customErrorMsg )
{
    vr::EVRSettingsError error;
    vr::VRSettings()->SetFloat( pchSection, pchSettingsKey, flValue, &error );
    handleErrors( errorType, pchSettingsKey, error, customErrorMsg );
}

} // namespace ivrsettings
