#include "input_sender.h"
#include <vector>
#include <Windows.h>
#include <cctype>

WORD convertToVirtualKeycode( const Token token )
{
    if ( isLiteralKey( token ) )
    {
        return static_cast<WORD>( VkKeyScan( static_cast<int>( token ) ) );
    }

    switch ( token )
    {
    case Token::KEY_F1:
        return VK_F1;
    case Token::KEY_F2:
        return VK_F2;
    case Token::KEY_F3:
        return VK_F3;
    case Token::KEY_F4:
        return VK_F4;
    case Token::KEY_F5:
        return VK_F5;
    case Token::KEY_F6:
        return VK_F6;
    case Token::KEY_F7:
        return VK_F7;
    case Token::KEY_F8:
        return VK_F8;
    case Token::KEY_F9:
        return VK_F9;
    case Token::KEY_F10:
        return VK_F10;
    case Token::KEY_F11:
        return VK_F11;
    case Token::KEY_F12:
        return VK_F12;
    case Token::KEY_F13:
        return VK_F13;
    case Token::KEY_F14:
        return VK_F14;
    case Token::KEY_F15:
        return VK_F15;
    case Token::KEY_F16:
        return VK_F16;
    case Token::KEY_F17:
        return VK_F17;
    case Token::KEY_F18:
        return VK_F18;
    case Token::KEY_F19:
        return VK_F19;

    case Token::KEY_BACKSPACE:
        return VK_BACK;
    case Token::KEY_SPACE:
        return VK_SPACE;
    case Token::KEY_TAB:
        return VK_TAB;
    case Token::KEY_ESC:
        return VK_ESCAPE;
    case Token::KEY_INS:
        return VK_INSERT;
    case Token::KEY_DEL:
        return VK_DELETE;
    case Token::KEY_END:
        return VK_END;
    case Token::KEY_PGDN:
        return VK_NEXT;
    case Token::KEY_PGUP:
        return VK_PRIOR;
    case Token::KEY_CAPS:
        return VK_CAPITAL;
    case Token::KEY_PRNSCRN:
        return VK_SNAPSHOT;
    case Token::KEY_PAUSE:
        return VK_PAUSE;
    case Token::KEY_SCRLOCK:
        return VK_SCROLL;
    case Token::KEY_LEFTARROW:
        return VK_LEFT;
    case Token::KEY_RIGHTARROW:
        return VK_RIGHT;
    case Token::KEY_UPARROW:
        return VK_UP;
    case Token::KEY_DOWNARROW:
        return VK_DOWN;
    case Token::KEY_KPSLASH:
        return VK_DIVIDE;
    case Token::KEY_KPSTAR:
        return VK_MULTIPLY;
    case Token::KEY_KPMINUS:
        return VK_SUBTRACT;
    case Token::KEY_KPPLUS:
        return VK_ADD;
    case Token::KEY_ENTER:
        return VK_RETURN;

    case Token::MODIFIER_CTRL:
        return VK_CONTROL;
    case Token::MODIFIER_ALT:
        return VK_MENU;
    case Token::MODIFIER_SHIFT:
        return VK_SHIFT;
    case Token::MODIFIER_RSHIFT:
        return VK_RSHIFT;
    case Token::MODIFIER_SUPER:
        return VK_LWIN;
    case Token::MODIFIER_TILDE:
        return VK_OEM_3;
    case Token::KEY_BACKSLASH:
        return VK_OEM_5;

    default:
        return 0;
    }
}
void initOsSystems()
{
    // Intentionally blank, no setup needed on Windows
}

void shutdownOsSystems()
{
    // Intentionally blank, no shutdown needed on Windows
}

INPUT createInputStruct( const WORD virtualKeyCode,
                         const KeyStatus keyStatus ) noexcept
{
    INPUT input = {};

    input.type = INPUT_KEYBOARD;

    if ( virtualKeyCode == VK_RSHIFT )
    {
        input.ki.wVk = VK_SHIFT;
        input.ki.wScan = 0x36;

        if ( keyStatus == KeyStatus::Up )
        {
            input.ki.dwFlags = KEYEVENTF_KEYUP | KEYEVENTF_SCANCODE;
        }
        else if ( keyStatus == KeyStatus::Down )
        {
            input.ki.dwFlags = 0 | KEYEVENTF_SCANCODE;
        }
    }
    else
    {
        input.ki.wVk = virtualKeyCode;

        if ( keyStatus == KeyStatus::Up )
        {
            input.ki.dwFlags = KEYEVENTF_KEYUP;
        }
        else if ( keyStatus == KeyStatus::Down )
        {
            input.ki.dwFlags = 0;
        }
    }

    return input;
}

void sendKeyboardInputRaw( std::vector<INPUT> inputs )
{
    const auto success = SendInput( static_cast<unsigned int>( inputs.size() ),
                                    inputs.data(),
                                    sizeof( INPUT ) );

    if ( ( inputs.size() > 0 ) && !success )
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

void sendKeyPress( const Token token, const KeyStatus status )
{
    std::vector<INPUT> v
        = { createInputStruct( convertToVirtualKeycode( token ), status ) };
    sendKeyboardInputRaw( v );
}
