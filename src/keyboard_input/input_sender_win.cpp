#include "input_sender.h"
#include <vector>
#include <Windows.h>
#include <cctype>

WORD convertToVirtualKeycode( const Token token )
{
    if ( isalnum( static_cast<int>( token ) ) )
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

    case Token::MODIFIER_CTRL:
        return VK_CONTROL;
    case Token::MODIFIER_ALT:
        return VK_MENU;
    case Token::MODIFIER_SHIFT:
        return VK_SHIFT;
    case Token::MODIFIER_SUPER:
        return VK_LWIN;

    default:
        // LOG incorrect symbol
        return 0;
    }
}

INPUT createInputStruct( const WORD virtualKeyCode,
                         const KeyStatus keyStatus ) noexcept
{
    INPUT input = {};

    input.type = INPUT_KEYBOARD;

    input.ki.wVk = virtualKeyCode;

    if ( keyStatus == KeyStatus::Up )
    {
        input.ki.dwFlags = KEYEVENTF_KEYUP;
    }
    else if ( keyStatus == KeyStatus::Down )
    {
        input.ki.dwFlags = 0;
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

void sendTokensAsInput( const std::vector<Token> tokens )
{
    std::vector<INPUT> inputs = {};
    std::vector<Token> heldInputs = {};
    for ( const auto& token : tokens )
    {
        if ( isModifier( token ) )
        {
            inputs.push_back( createInputStruct(
                convertToVirtualKeycode( token ), KeyStatus::Down ) );
            heldInputs.push_back( token );
            continue;
        }

        if ( token == Token::TOKEN_NEW_SEQUENCE )
        {
            for ( const auto& h : heldInputs )
            {
                inputs.push_back( createInputStruct(
                    convertToVirtualKeycode( h ), KeyStatus::Up ) );
            }
            heldInputs.clear();
            continue;
        }

        if ( isLiteral( token ) )
        {
            inputs.push_back( createInputStruct(
                convertToVirtualKeycode( token ), KeyStatus::Down ) );
            inputs.push_back( createInputStruct(
                convertToVirtualKeycode( token ), KeyStatus::Up ) );
            continue;
        }
    }

    sendKeyboardInputRaw( inputs );
    // send inputs
}
