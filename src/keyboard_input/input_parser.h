#pragma once
#include <string>
#include <vector>
#include <cctype>
#include <optional>
#include <easylogging++.h>

enum class Token
{

    KEY_F1,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    KEY_F6,
    KEY_F7,
    KEY_F8,
    KEY_F9,
    KEY_F10,
    KEY_F11,
    KEY_F12,
    KEY_F13,
    KEY_F14,
    KEY_F15,
    KEY_F16,
    KEY_F17,
    KEY_F18,
    KEY_F19,

    KEY_BACKSPACE = 150,
    KEY_SPACE,
    KEY_TAB,
    KEY_ESC,
    KEY_INS,
    KEY_DEL,
    KEY_END,
    KEY_PGDN,
    KEY_PGUP,
    KEY_CAPS,
    KEY_PRNSCRN,
    KEY_PAUSE,
    KEY_SCRLOCK,
    KEY_LEFTARROW,
    KEY_RIGHTARROW,
    KEY_UPARROW,
    KEY_DOWNARROW,
    KEY_KPSLASH,
    KEY_KPSTAR,
    KEY_KPMINUS,
    KEY_KPPLUS,
    KEY_ENTER,
    KEY_BACKSLASH,

    // Misc tokens
    TOKEN_NEW_SEQUENCE,
    TOKEN_NO_KEYUP_NEXT,

    // Modifiers
    MODIFIER_CTRL,
    MODIFIER_ALT,
    MODIFIER_SHIFT,
    MODIFIER_RSHIFT,
    MODIFIER_SUPER,
    MODIFIER_TILDE,

    // Literals
    KEY_a = 'a',
    KEY_b = 'b',
    KEY_c = 'c',
    KEY_d = 'd',
    KEY_e = 'e',
    KEY_f = 'f',
    KEY_g = 'g',
    KEY_h = 'h',
    KEY_i = 'i',
    KEY_j = 'j',
    KEY_k = 'k',
    KEY_l = 'l',
    KEY_m = 'm',
    KEY_n = 'n',
    KEY_o = 'o',
    KEY_p = 'p',
    KEY_q = 'q',
    KEY_r = 'r',
    KEY_s = 's',
    KEY_t = 't',
    KEY_u = 'u',
    KEY_v = 'v',
    KEY_w = 'w',
    KEY_x = 'x',
    KEY_y = 'y',
    KEY_z = 'z',

    KEY_0 = '0',
    KEY_1 = '1',
    KEY_2 = '2',
    KEY_3 = '3',
    KEY_4 = '4',
    KEY_5 = '5',
    KEY_6 = '6',
    KEY_7 = '7',
    KEY_8 = '8',
    KEY_9 = '9',
};

std::vector<Token>
    ParseKeyboardInputsToTokens( const std::string inputs ) noexcept;
std::vector<Token>
    removeIncorrectTokens( const std::vector<Token>& tokens ) noexcept;

bool isModifier( const Token token ) noexcept;
bool isLiteral( const Token token ) noexcept;
bool isLiteralKey( const Token token ) noexcept;
