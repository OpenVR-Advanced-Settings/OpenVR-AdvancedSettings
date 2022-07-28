#pragma once

#include <openvr.h>
#include <iostream>
#include <string>
#include <easylogging++.h>
#include <utility>
#include <map>

/* Wrapper For OpenVR's IVR settings class, allows us to do our error logging
 * while also minimizing code
 *
 */
namespace ovr_system_wrapper
{
// might make sense to mirror ovr's errors exactly, but for now this is more
// than sufficient

enum class SystemError
{
    NoError,
    UndefinedError,

};

std::pair<SystemError, bool>
    getBoolTrackedProperty( int deviceIndex,
                            vr::TrackedDeviceProperty property,
                            std::string customErrorMsg = "" );

std::pair<SystemError, int>
    getInt32TrackedProperty( int deviceIndex,
                             vr::TrackedDeviceProperty property,
                             std::string customErrorMsg = "" );

std::pair<SystemError, float>
    getFloatTrackedProperty( int deviceIndex,
                             vr::TrackedDeviceProperty property,
                             std::string customErrorMsg = "" );
std::pair<SystemError, std::string>
    getStringTrackedProperty( int deviceIndex,
                              vr::TrackedDeviceProperty property,
                              std::string customErrorMsg = "" );

SystemError handleTrackedPropertyErrors( vr::TrackedDeviceProperty tdp,
                                         vr::ETrackedPropertyError error,
                                         std::string customErrorMsg );
std::string getPropertyFromEnum( vr::TrackedDeviceProperty );

bool deviceConnected( int Index );

vr::ETrackedDeviceClass getDeviceClass( int index );

std::vector<int> getAllConnectedDevices( bool onlyWearable = false );
std::string getDeviceName( int index );
std::string getControllerName();

} // namespace ovr_system_wrapper
