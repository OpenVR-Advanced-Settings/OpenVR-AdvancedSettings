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
    const auto tokens = std::vector<Token>{ Token::TOKEN_NO_KEYUP_NEXT,
                                            Token::MODIFIER_ALT,
                                            Token::KEY_TAB };

    sendTokensAsInput( tokens );
}

void sendKeyboardAltEnter()
{
    const auto tokens = std::vector<Token>{ Token::TOKEN_NO_KEYUP_NEXT,
                                            Token::MODIFIER_ALT,
                                            Token::KEY_ENTER };

    sendTokensAsInput( tokens );
}

void sendKeyboardCtrlC()
{
    const auto tokens = std::vector<Token>{ Token::TOKEN_NO_KEYUP_NEXT,
                                            Token::MODIFIER_CTRL,
                                            Token::KEY_c };

    sendTokensAsInput( tokens );
}

void sendKeyboardCtrlV()
{
    const auto tokens = std::vector<Token>{ Token::TOKEN_NO_KEYUP_NEXT,
                                            Token::MODIFIER_CTRL,
                                            Token::KEY_v };

    sendTokensAsInput( tokens );
}

void sendKeyboardRShiftTilde1()
{
    const auto tokens = std::vector<Token>{
        Token::TOKEN_NO_KEYUP_NEXT, Token::MODIFIER_RSHIFT,
        Token::TOKEN_NO_KEYUP_NEXT, Token::MODIFIER_TILDE,
        Token::TOKEN_NO_KEYUP_NEXT, Token::KEY_1
    };
    sendTokensAsInput( tokens );
}

void sendKeyboardRShiftTilde2()
{
    const auto tokens = std::vector<Token>{
        Token::TOKEN_NO_KEYUP_NEXT, Token::MODIFIER_RSHIFT,
        Token::TOKEN_NO_KEYUP_NEXT, Token::MODIFIER_TILDE,
        Token::TOKEN_NO_KEYUP_NEXT, Token::KEY_2
    };
    sendTokensAsInput( tokens );
}

void sendKeyboardRShiftTilde3()
{
    const auto tokens = std::vector<Token>{
        Token::TOKEN_NO_KEYUP_NEXT, Token::MODIFIER_RSHIFT,
        Token::TOKEN_NO_KEYUP_NEXT, Token::MODIFIER_TILDE,
        Token::TOKEN_NO_KEYUP_NEXT, Token::KEY_3
    };
    sendTokensAsInput( tokens );
}

void sendKeyboardRShiftTilde4()
{
    const auto tokens = std::vector<Token>{
        Token::TOKEN_NO_KEYUP_NEXT, Token::MODIFIER_RSHIFT,
        Token::TOKEN_NO_KEYUP_NEXT, Token::MODIFIER_TILDE,
        Token::TOKEN_NO_KEYUP_NEXT, Token::KEY_4
    };
    sendTokensAsInput( tokens );
}

void sendKeyboardRShiftTilde5()
{
    const auto tokens = std::vector<Token>{
        Token::TOKEN_NO_KEYUP_NEXT, Token::MODIFIER_RSHIFT,
        Token::TOKEN_NO_KEYUP_NEXT, Token::MODIFIER_TILDE,
        Token::TOKEN_NO_KEYUP_NEXT, Token::KEY_5
    };
    sendTokensAsInput( tokens );
}

void sendKeyboardRShiftTilde6()
{
    const auto tokens = std::vector<Token>{
        Token::TOKEN_NO_KEYUP_NEXT, Token::MODIFIER_RSHIFT,
        Token::TOKEN_NO_KEYUP_NEXT, Token::MODIFIER_TILDE,
        Token::TOKEN_NO_KEYUP_NEXT, Token::KEY_6
    };
    sendTokensAsInput( tokens );
}

void sendKeyboardRShiftTilde7()
{
    const auto tokens = std::vector<Token>{
        Token::TOKEN_NO_KEYUP_NEXT, Token::MODIFIER_RSHIFT,
        Token::TOKEN_NO_KEYUP_NEXT, Token::MODIFIER_TILDE,
        Token::TOKEN_NO_KEYUP_NEXT, Token::KEY_7
    };
    sendTokensAsInput( tokens );
}

void sendKeyboardRShiftTilde8()
{
    const auto tokens = std::vector<Token>{
        Token::TOKEN_NO_KEYUP_NEXT, Token::MODIFIER_RSHIFT,
        Token::TOKEN_NO_KEYUP_NEXT, Token::MODIFIER_TILDE,
        Token::TOKEN_NO_KEYUP_NEXT, Token::KEY_8
    };
    sendTokensAsInput( tokens );
}

void sendKeyboardRShiftTilde9()
{
    const auto tokens = std::vector<Token>{
        Token::TOKEN_NO_KEYUP_NEXT, Token::MODIFIER_RSHIFT,
        Token::TOKEN_NO_KEYUP_NEXT, Token::MODIFIER_TILDE,
        Token::TOKEN_NO_KEYUP_NEXT, Token::KEY_9
    };
    sendTokensAsInput( tokens );
}

void sendKeyboardRShiftTilde0()
{
    const auto tokens = std::vector<Token>{
        Token::TOKEN_NO_KEYUP_NEXT, Token::MODIFIER_RSHIFT,
        Token::TOKEN_NO_KEYUP_NEXT, Token::MODIFIER_TILDE,
        Token::TOKEN_NO_KEYUP_NEXT, Token::KEY_0
    };
    sendTokensAsInput( tokens );
}

void sendKeyboardRShiftTilde1Delayed()
{
    const auto tokens = std::vector<Token>{ Token::MODIFIER_RSHIFT,
                                            Token::MODIFIER_TILDE,
                                            Token::KEY_1 };
    sendTokensAsInput( tokens );
}

void sendKeyboardRShiftTilde2Delayed()
{
    const auto tokens = std::vector<Token>{ Token::MODIFIER_RSHIFT,
                                            Token::MODIFIER_TILDE,
                                            Token::KEY_2 };
    sendTokensAsInput( tokens );
}

void sendKeyboardRShiftTilde3Delayed()
{
    const auto tokens = std::vector<Token>{ Token::MODIFIER_RSHIFT,
                                            Token::MODIFIER_TILDE,
                                            Token::KEY_3 };
    sendTokensAsInput( tokens );
}

void sendKeyboardRShiftTilde4Delayed()
{
    const auto tokens = std::vector<Token>{ Token::MODIFIER_RSHIFT,
                                            Token::MODIFIER_TILDE,
                                            Token::KEY_4 };
    sendTokensAsInput( tokens );
}

void sendKeyboardRShiftTilde5Delayed()
{
    const auto tokens = std::vector<Token>{ Token::MODIFIER_RSHIFT,
                                            Token::MODIFIER_TILDE,
                                            Token::KEY_5 };
    sendTokensAsInput( tokens );
}

void sendKeyboardRShiftTilde6Delayed()
{
    const auto tokens = std::vector<Token>{ Token::MODIFIER_RSHIFT,
                                            Token::MODIFIER_TILDE,
                                            Token::KEY_6 };
    sendTokensAsInput( tokens );
}

void sendKeyboardRShiftTilde7Delayed()
{
    const auto tokens = std::vector<Token>{ Token::MODIFIER_RSHIFT,
                                            Token::MODIFIER_TILDE,
                                            Token::KEY_7 };
    sendTokensAsInput( tokens );
}

void sendKeyboardRShiftTilde8Delayed()
{
    const auto tokens = std::vector<Token>{ Token::MODIFIER_RSHIFT,
                                            Token::MODIFIER_TILDE,
                                            Token::KEY_8 };
    sendTokensAsInput( tokens );
}

void sendKeyboardRShiftTilde9Delayed()
{
    const auto tokens = std::vector<Token>{ Token::MODIFIER_RSHIFT,
                                            Token::MODIFIER_TILDE,
                                            Token::KEY_9 };
    sendTokensAsInput( tokens );
}

void sendKeyboardRShiftTilde0Delayed()
{
    const auto tokens = std::vector<Token>{ Token::MODIFIER_RSHIFT,
                                            Token::MODIFIER_TILDE,
                                            Token::KEY_0 };
    sendTokensAsInput( tokens );
}

} // namespace keyboardinput
