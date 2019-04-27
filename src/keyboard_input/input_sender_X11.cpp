#include "input_sender.h"
#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <X11/extensions/XTest.h>

unsigned int tokenToKeyCode( const Token token, Display* const display )
{
    if ( isalnum( static_cast<int>( token ) ) )
    {
        return XKeysymToKeycode( display, static_cast<KeySym>( token ) );
    }

    switch ( token )
    {
    case Token::KEY_F1:
        return XK_F1;
    case Token::KEY_F2:
        return XK_F2;
    case Token::KEY_F3:
        return XK_F3;
    case Token::KEY_F4:
        return XK_F4;
    case Token::KEY_F5:
        return XK_F5;
    case Token::KEY_F6:
        return XK_F6;
    case Token::KEY_F7:
        return XK_F7;
    case Token::KEY_F8:
        return XK_F8;
    case Token::KEY_F9:
        return XK_F9;

    case Token::KEY_BACKSPACE:
        return XK_BackSpace;
    case Token::KEY_SPACE:
        return XK_space;
    case Token::KEY_TAB:
        return XK_Tab;
    case Token::KEY_ESC:
        return XK_Escape;
    case Token::KEY_INS:
        return XK_Insert;
    case Token::KEY_DEL:
        return XK_Delete;
    case Token::KEY_END:
        return XK_End;
    case Token::KEY_PGDN:
        return XK_Page_Down;
    case Token::KEY_PGUP:
        return XK_Page_Up;
    case Token::KEY_CAPS:
        return XK_Caps_Lock;
    case Token::KEY_PRNSCRN:
        return XK_Print;
    case Token::KEY_PAUSE:
        return XK_Pause;
    case Token::KEY_SCRLOCK:
        return XK_Scroll_Lock;
    case Token::KEY_LEFTARROW:
        return XK_Left;
    case Token::KEY_RIGHTARROW:
        return XK_Right;
    case Token::KEY_UPARROW:
        return XK_Up;
    case Token::KEY_DOWNARROW:
        return XK_Down;
    case Token::KEY_KPSLASH:
        return XK_KP_Divide;
    case Token::KEY_KPSTAR:
        return XK_KP_Multiply;
    case Token::KEY_KPMINUS:
        return XK_KP_Subtract;
    case Token::KEY_KPPLUS:
        return XK_KP_Add;

    case Token::MODIFIER_CTRL:
        return XK_Control_L;
    case Token::MODIFIER_ALT:
        return XK_Alt_L;
    case Token::MODIFIER_SHIFT:
        return XK_Shift_L;
    case Token::MODIFIER_SUPER:
        return XK_Super_L;

    default:
        // LOG incorrect symbol
        return 0;
    }
}

void sendKeyPress( const Token token,
                   const KeyStatus status,
                   Display* const display )
{
    bool keyDown = false;

    if ( status == KeyStatus::Up )
    {
        keyDown = false;
    }
    else
    {
        keyDown = true;
    }

    XTestFakeKeyEvent( display, tokenToKeyCode( token, display ), keyDown, 0 );
}

void sendTokenAsInput( const std::vector<Token> tokens )
{
    Display* const display = XOpenDisplay( nullptr );
    XTestGrabControl( display, True );

    std::vector<Token> heldInputs = {};
    for ( const auto& token : tokens )
    {
        if ( isModifier( token ) )
        {
            sendKeyPress( token, KeyStatus::Down, display );
            continue;
        }

        if ( token == Token::TOKEN_NEW_SEQUENCE )
        {
            for ( const auto& h : heldInputs )
            {
                sendKeyPress( h, KeyStatus::Up, display );
            }
            heldInputs.clear();
            continue;
        }

        if ( isLiteral( token ) )
        {
            sendKeyPress( token, KeyStatus::Down, display );
            sendKeyPress( token, KeyStatus::Up, display );
            continue;
        }
    }

    XSync( display, False );
    XTestGrabControl( display, False );
}
