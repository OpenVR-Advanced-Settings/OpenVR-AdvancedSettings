#pragma once

#include <openvr.h>
#include <iostream>
#include <string>
#include <easylogging++.h>

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

bool getBool( const char* pchSection,
              const char* pchSettingsKey,
              logType errorType,
              std::string customErrorMsg = "" );

int getInt32( const char* pchSection,
              const char* pchSettingsKey,
              logType errorType,
              std::string customErrorMsg = "" );

float getFloat( const char* pchSection,
                const char* pchSettingsKey,
                logType errorType,
                std::string customErrorMsg = "" );
// TODO string

void setBool( const char* pchSection,
              const char* pchSettingsKey,
              bool bValue,
              logType errorType,
              std::string customErrorMsg = "" );
void setInt32( const char* pchSection,
               const char* pchSettingsKey,
               int nValue,
               logType errorType,
               std::string customErrorMsg = "" );
void setFloat( const char* pchSection,
               const char* pchSettingsKey,
               float flValue,
               logType errorType,
               std::string customErrorMsg = "" );

void handleErrors( logType errorType,
                   const char* pchSettingsKey,
                   vr::EVRSettingsError,
                   std::string customErrorMsg );
} // namespace ivrsettings
