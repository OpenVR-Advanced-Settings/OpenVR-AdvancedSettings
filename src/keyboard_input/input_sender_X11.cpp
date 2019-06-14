#include "input_sender.h"
#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <X11/extensions/XTest.h>

unsigned int tokenToKeySym( const Token token )
{
    switch ( token )
    {
    case Token::KEY_a:
        return XK_a;
    case Token::KEY_b:
        return XK_b;
    case Token::KEY_c:
        return XK_c;
    case Token::KEY_d:
        return XK_d;
    case Token::KEY_e:
        return XK_e;
    case Token::KEY_f:
        return XK_f;
    case Token::KEY_g:
        return XK_g;
    case Token::KEY_h:
        return XK_h;
    case Token::KEY_i:
        return XK_i;
    case Token::KEY_j:
        return XK_j;
    case Token::KEY_k:
        return XK_k;
    case Token::KEY_l:
        return XK_l;
    case Token::KEY_m:
        return XK_m;
    case Token::KEY_n:
        return XK_n;
    case Token::KEY_o:
        return XK_o;
    case Token::KEY_p:
        return XK_p;
    case Token::KEY_q:
        return XK_q;
    case Token::KEY_r:
        return XK_r;
    case Token::KEY_s:
        return XK_s;
    case Token::KEY_t:
        return XK_t;
    case Token::KEY_u:
        return XK_u;
    case Token::KEY_v:
        return XK_v;
    case Token::KEY_w:
        return XK_w;
    case Token::KEY_x:
        return XK_x;
    case Token::KEY_y:
        return XK_y;
    case Token::KEY_z:
        return XK_z;

    case Token::KEY_0:
        return XK_0;
    case Token::KEY_1:
        return XK_1;
    case Token::KEY_2:
        return XK_2;
    case Token::KEY_3:
        return XK_3;
    case Token::KEY_4:
        return XK_4;
    case Token::KEY_5:
        return XK_5;
    case Token::KEY_6:
        return XK_6;
    case Token::KEY_7:
        return XK_7;
    case Token::KEY_8:
        return XK_8;
    case Token::KEY_9:
        return XK_9;

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
    case Token::KEY_ENTER:
        return XK_Return;

    case Token::MODIFIER_CTRL:
        return XK_Control_L;
    case Token::MODIFIER_ALT:
        return XK_Alt_L;
    case Token::MODIFIER_SHIFT:
        return XK_Shift_L;
    case Token::MODIFIER_SUPER:
        return XK_Super_L;

    default:
        return 0;
    }
}

void sendKeyPress( const Token token,
                   const KeyStatus status,
                   Display* const display )
{
    bool keyDown = false;

    if ( status == KeyStatus::Down )
    {
        keyDown = true;
    }

    XTestFakeKeyEvent( display,
                       XKeysymToKeycode( display, tokenToKeySym( token ) ),
                       keyDown,
                       0 );
}

void sendTokensAsInput( const std::vector<Token> tokens )
{
    Display* const display = XOpenDisplay( nullptr );
    XTestGrabControl( display, True );

    std::vector<Token> heldInputs = {};
    for ( const auto& token : tokens )
    {
        if ( isModifier( token ) )
        {
            sendKeyPress( token, KeyStatus::Down, display );
            heldInputs.push_back( token );
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

    if ( !heldInputs.empty() )
    {
        for ( const auto& h : heldInputs )
        {
            sendKeyPress( h, KeyStatus::Up, display );
        }
    }

    XSync( display, False );
    XTestGrabControl( display, False );
}
