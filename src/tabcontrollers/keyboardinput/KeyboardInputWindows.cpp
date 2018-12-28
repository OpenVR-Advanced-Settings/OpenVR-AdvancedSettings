#include "KeyboardInputWindows.h"
#include <Windows.h>
#include "easylogging++.h"

namespace advsettings
{
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

void sendKeyboardInputRaw( int inputCount, LPINPUT input )
{
    if ( ( inputCount > 0 )
         && !SendInput(
                static_cast<UINT>( inputCount ), input, sizeof( INPUT ) ) )
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
