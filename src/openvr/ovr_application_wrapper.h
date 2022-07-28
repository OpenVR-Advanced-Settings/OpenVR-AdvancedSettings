#pragma once

#include <openvr.h>
#include <iostream>
#include <string>
#include <easylogging++.h>
#include <utility>
#include <map>

/* Wrapper For OpenVR's IVR application class, allows us to do our error logging
 * while also minimizing code
 *
 */
namespace ovr_application_wrapper
{
// might make sense to mirror ovr's errors exactly, but for now this is more
// than sufficient

enum class ApplicationError
{
    NoError,
    UndefinedError,

};

std::string getSceneAppID();

} // namespace ovr_application_wrapper
