#pragma once

#include <openvr.h>
#include <iostream>
#include <string>
#include <easylogging++.h>
#include <utility>

/* Wrapper For OpenVR's IVR settings class, allows us to do our error logging
 * while also minimizing code
 *
 */
namespace ivrsettings
{
enum class logType
{
    warn,
    err,
    bug

};

// might make sense to mirror ovr's errors exactly, but for now this is more
// than sufficient
enum class settingsError
{
    noErr,
    undefErr,

};

std::pair<settingsError, bool> getBool( const char* pchSection,
                                        const char* pchSettingsKey,
                                        logType errorType,
                                        std::string customErrorMsg = "" );

std::pair<settingsError, int> getInt32( const char* pchSection,
                                        const char* pchSettingsKey,
                                        logType errorType,
                                        std::string customErrorMsg = "" );

std::pair<settingsError, float> getFloat( const char* pchSection,
                                          const char* pchSettingsKey,
                                          logType errorType,
                                          std::string customErrorMsg = "" );
// TODO string

settingsError setBool( const char* pchSection,
                       const char* pchSettingsKey,
                       bool bValue,
                       logType errorType,
                       std::string customErrorMsg = "" );
settingsError setInt32( const char* pchSection,
                        const char* pchSettingsKey,
                        int nValue,
                        logType errorType,
                        std::string customErrorMsg = "" );
settingsError setFloat( const char* pchSection,
                        const char* pchSettingsKey,
                        float flValue,
                        logType errorType,
                        std::string customErrorMsg = "" );

settingsError handleErrors( logType errorType,
                            const char* pchSettingsKey,
                            vr::EVRSettingsError,
                            std::string customErrorMsg );
} // namespace ivrsettings
