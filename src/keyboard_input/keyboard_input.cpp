#include "keyboard_input.h"
#include "src/keyboard_input/input_sender.h"

namespace keyboardinput
{
void sendKeyboardInput( QString input )
{
    sendStringAsInput( input.toStdString() );
}

void sendKeyboardEnter()
{
    const auto tokens = std::vector<Token>{ Token::KEY_ENTER };

    sendTokensAsInput( tokens );
}

void sendKeyboardBackspace( const int count )
{
    const auto tokens = std::vector<Token>{ Token::KEY_BACKSPACE };
    for ( int i = 0; i < count; ++i )
    {
        sendTokensAsInput( tokens );
    }
}

void sendKeyboardAltTab()
{
    const auto tokens
        = std::vector<Token>{ Token::MODIFIER_ALT, Token::KEY_TAB };

    sendTokensAsInput( tokens );
}

void sendKeyboardAltEnter()
{
    const auto tokens
        = std::vector<Token>{ Token::MODIFIER_ALT, Token::KEY_ENTER };

    sendTokensAsInput( tokens );
}

void sendKeyboardCtrlC()
{
    const auto tokens
        = std::vector<Token>{ Token::MODIFIER_CTRL, Token::KEY_c };

    sendTokensAsInput( tokens );
}

void sendKeyboardCtrlV()
{
    const auto tokens
        = std::vector<Token>{ Token::MODIFIER_CTRL, Token::KEY_v };

    sendTokensAsInput( tokens );
}

} // namespace keyboardinput
