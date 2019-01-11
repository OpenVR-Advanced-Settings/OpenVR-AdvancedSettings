#include "KeyboardInputWindows.h"
#include <Windows.h>
#include "easylogging++.h"

namespace advsettings
{
/*!
Represents the state of a keyboard button press. Can be either Up or Down.

Implemented because neither Qt nor Windows.h included a sensible key state enum.
*/
enum class KeyStatus
{
    Up,
    Down,
};

void fillKiStruct( INPUT& ip, WORD scanCode, bool keyup )
{
    ip.type = INPUT_KEYBOARD;
    ip.ki.wVk = scanCode;
    if ( keyup )
    {
        ip.ki.dwFlags = KEYEVENTF_KEYUP;
    }
    else
    {
        ip.ki.dwFlags = 0;
    }
    ip.ki.wScan = 0;
    ip.ki.dwExtraInfo = 0;
    ip.ki.time = 0;
};

void sendKeyboardInputRaw( const int inputCount, const LPINPUT input )
{
    const auto success
        = SendInput( static_cast<UINT>( inputCount ), input, sizeof( INPUT ) );

    if ( ( inputCount > 0 ) && !success )
    {
        char* err;
        auto errCode = GetLastError();
        if ( !FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER
                                 | FORMAT_MESSAGE_FROM_SYSTEM,
                             nullptr,
                             errCode,
                             MAKELANGID( LANG_NEUTRAL,
                                         SUBLANG_DEFAULT ), // default language
                             reinterpret_cast<LPTSTR>( &err ),
                             0,
                             nullptr ) )
        {
            LOG( ERROR )
                << "Error calling SendInput(): Could not get error message ("
                << errCode << ")";
        }
        else
        {
            LOG( ERROR ) << "Error calling SendInput(): " << err;
        }
    }
}

/*!
Returns an INPUT struct with the corresponding virtualKeyCode and keyStatus set.

All fields except ki.wVk and ki.dwFlags are zero.
The INPUT struct is used with the SendInput Windows function.
Official Docs:
https://docs.microsoft.com/en-us/windows/desktop/api/winuser/ns-winuser-taginput
*/
INPUT createInputStruct( const WORD virtualKeyCode, const KeyStatus keyStatus )
{
    // Zero init to ensure random data doesn't muck something up.
    INPUT input = {};

    input.type = INPUT_KEYBOARD;

    input.ki.wVk = virtualKeyCode;

    if ( keyStatus == KeyStatus::Up )
    {
        input.ki.dwFlags = KEYEVENTF_KEYUP;
    }
    else if ( keyStatus == KeyStatus::Down )
    {
        // Struct is already zero initialized, but this is here for clarity.
        // Compiler will likely sort this out, otherwise the performance hit is
        // negligible.
        // There is no corresponding KEYDOWN event, you just don't
        // set KEYEVENTF_KEYUP.
        input.ki.dwFlags = 0;
    }

    // The sizeof(INPUT) on MSVC is 28. Returning by value is a non-issue
    // compared to the simplicity of not having to pass a ref to an already
    // existing INPUT struct, and possibly forgetting to zero it out.
    return input;
}

/*!
Sends a key press to Windows. The virtualKeyCode is a Windows specific define
found in <windows.h>.

Virtual Key Codes offical docs:
https://docs.microsoft.com/en-us/windows/desktop/inputdev/virtual-key-codes
*/
void sendKeyPressAndRelease( const WORD virtualKeyCode )
{
    const auto press = createInputStruct( virtualKeyCode, KeyStatus::Down );
    const auto release = createInputStruct( virtualKeyCode, KeyStatus::Up );

    constexpr auto numberOfActions = 2;

    INPUT actions[numberOfActions] = { press, release };

    sendKeyboardInputRaw( numberOfActions, actions );
}

void KeyboardInputWindows::sendKeyboardInput( QString input )
{
    int len = input.length();
    if ( len > 0 )
    {
        LPINPUT ips = new INPUT[static_cast<unsigned int>( len ) * 5 + 3];
        int ii = 0;
        bool shiftPressed = false;
        bool ctrlPressed = false;
        bool altPressed = false;
        for ( int i = 0; i < len; i++ )
        {
            short tmp = VkKeyScan( input.at( i ).toLatin1() );
            WORD c = tmp & 0xFF;
            short shiftState = tmp >> 8;
            bool isShift = shiftState & 1;
            bool isCtrl = shiftState & 2;
            bool isAlt = shiftState & 4;
            if ( isShift && !shiftPressed )
            {
                fillKiStruct( ips[ii++], VK_SHIFT, false );
                shiftPressed = true;
            }
            else if ( !isShift && shiftPressed )
            {
                fillKiStruct( ips[ii++], VK_SHIFT, true );
                shiftPressed = false;
            }
            if ( isCtrl && !ctrlPressed )
            {
                fillKiStruct( ips[ii++], VK_CONTROL, false );
                ctrlPressed = true;
            }
            else if ( !isCtrl && ctrlPressed )
            {
                fillKiStruct( ips[ii++], VK_CONTROL, true );
                ctrlPressed = false;
            }
            if ( isAlt && !altPressed )
            {
                fillKiStruct( ips[ii++], VK_MENU, false );
                altPressed = true;
            }
            else if ( !isAlt && altPressed )
            {
                fillKiStruct( ips[ii++], VK_MENU, true );
                altPressed = false;
            }
            fillKiStruct( ips[ii++], c, false );
            fillKiStruct( ips[ii++], c, true );
        }
        if ( shiftPressed )
        {
            fillKiStruct( ips[ii++], VK_SHIFT, true );
            shiftPressed = false;
        }
        if ( ctrlPressed )
        {
            fillKiStruct( ips[ii++], VK_CONTROL, true );
            ctrlPressed = false;
        }
        if ( altPressed )
        {
            fillKiStruct( ips[ii++], VK_MENU, true );
            altPressed = false;
        }

        sendKeyboardInputRaw( ii, ips );
        delete[] ips;
    }
}

void KeyboardInputWindows::sendKeyboardEnter()
{
    LPINPUT ips = new INPUT[2];
    fillKiStruct( ips[0], VK_RETURN, false );
    fillKiStruct( ips[1], VK_RETURN, true );

    sendKeyboardInputRaw( 2, ips );
    delete[] ips;
}

void KeyboardInputWindows::sendKeyboardBackspace( int count )
{
    if ( count > 0 )
    {
        // We ensure that count is nonnegative, therefore safe cast.
        LPINPUT ips = new INPUT[static_cast<unsigned int>( count ) * 2];
        for ( int i = 0; i < count; i++ )
        {
            fillKiStruct( ips[2 * i], VK_BACK, false );
            fillKiStruct( ips[2 * i + 1], VK_BACK, true );
        }

        sendKeyboardInputRaw( count * 2, ips );
        delete[] ips;
    }
}

void KeyboardInputWindows::sendKeyboardAltTab()
{
    LPINPUT ips = new INPUT[4];
    // VK_MENU is alt
    fillKiStruct( ips[0], VK_MENU, false );
    fillKiStruct( ips[1], VK_TAB, false );
    fillKiStruct( ips[2], VK_TAB, true );
    fillKiStruct( ips[3], VK_MENU, true );

    sendKeyboardInputRaw( 4, ips );
    delete[] ips;
}

} // namespace advsettings
