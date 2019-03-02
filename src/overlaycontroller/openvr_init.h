#pragma once

namespace openvr_init
{
enum class OpenVrInitializationType
{
    Overlay,
    Utility,
};

/*!
Initializes OpenVR and checks validity of interface versions. Shuts down OpenVR
when destroyed.

This object should be declared above all other objects in the class file that
use OpenVR calls, since the order of declaration decides which objects are
initialized first.

The comment/tooltips have not been provided for the constructor/destructor since
they do not need explanation due to the straightforward nature of their
implementation.
*/
class OpenVR_Init
{
public:
    OpenVR_Init( const OpenVrInitializationType initType );
    ~OpenVR_Init();

    // As per the Rule of Five, the following constructors and assignments can
    // be created by the compiler automatically if they are used in the code.
    // The compiler will not be able to correctly create this class and it
    // wouldn't make sense to copy a class that just starts OpenVR.
    // They are explicitly deleted to make sure they aren't used, and to
    // document that they shouldn't be used.
    // Copy constructor
    OpenVR_Init( const OpenVR_Init& ) = delete;
    // Copy assignment
    const OpenVR_Init& operator=( const OpenVR_Init& ) = delete;
    // Move constructor
    OpenVR_Init( OpenVR_Init&& ) = delete;
    // Move assignment
    const OpenVR_Init operator=( OpenVR_Init&& ) = delete;
};
} // namespace openvr_init
