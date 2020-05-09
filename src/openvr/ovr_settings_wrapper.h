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
namespace ovr_settings_wrapper
{
// might make sense to mirror ovr's errors exactly, but for now this is more
// than sufficient
enum class SettingsError
{
    NoError,
    UndefinedError,

};

std::pair<SettingsError, bool> getBool( const char* pchSection,
                                        const char* pchSettingsKey,
                                        std::string customErrorMsg = "" );

std::pair<SettingsError, int> getInt32( const char* pchSection,
                                        const char* pchSettingsKey,
                                        std::string customErrorMsg = "" );

std::pair<SettingsError, float> getFloat( const char* pchSection,
                                          const char* pchSettingsKey,
                                          std::string customErrorMsg = "" );
// TODO string

SettingsError setBool( const char* pchSection,
                       const char* pchSettingsKey,
                       bool bValue,
                       std::string customErrorMsg = "" );
SettingsError setInt32( const char* pchSection,
                        const char* pchSettingsKey,
                        int nValue,
                        std::string customErrorMsg = "" );
SettingsError setFloat( const char* pchSection,
                        const char* pchSettingsKey,
                        float flValue,
                        std::string customErrorMsg = "" );

SettingsError removeSection( const char* pchSection,
                             std::string customErrorMsg = "" );
SettingsError removeKeyInSection( const char* pchSection,
                                  const char* pchSettingsKey,
                                  std::string customErrorMsg = "" );

SettingsError handleErrors( const char* pchSettingsKey,
                            vr::EVRSettingsError,
                            std::string customErrorMsg );
} // namespace ovr_settings_wrapper
