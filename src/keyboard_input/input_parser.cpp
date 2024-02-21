#include "input_parser.h"

std::optional<Token> getModifier( const char& character ) noexcept
{
    switch ( character )
    {
    case '^':
        return Token::MODIFIER_CTRL;
    case '*':
        return Token::MODIFIER_ALT;
    case '>':
        return Token::MODIFIER_SHIFT;
    case '#':
        return Token::MODIFIER_SUPER;
    default:
        return std::nullopt;
    }
}

std::optional<Token> getNumberOrLetter( const char& character ) noexcept
{
    if ( islower( character ) || isdigit( character ) )
    {
        return static_cast<Token>( character );
    }
    return std::nullopt;
}

std::optional<Token> getFunctionNumber( const char& character ) noexcept
{
    switch ( character )
    {
    case '1':
        return Token::KEY_F1;
    case '2':
        return Token::KEY_F2;
    case '3':
        return Token::KEY_F3;
    case '4':
        return Token::KEY_F4;
    case '5':
        return Token::KEY_F5;
    case '6':
        return Token::KEY_F6;
    case '7':
        return Token::KEY_F7;
    case '8':
        return Token::KEY_F8;
    case '9':
        return Token::KEY_F9;

    default:
        LOG( INFO ) << "Unknown function key number: " << character;
        return std::nullopt;
    }
}

std::optional<Token> getFunctionNumberExtended( const char& character ) noexcept
{
    switch ( character )
    {
    case '0':
        return Token::KEY_F10;
    case '1':
        return Token::KEY_F11;
    case '2':
        return Token::KEY_F12;
    case '3':
        return Token::KEY_F13;
    case '4':
        return Token::KEY_F14;
    case '5':
        return Token::KEY_F15;
    case '6':
        return Token::KEY_F16;
    case '7':
        return Token::KEY_F17;
    case '8':
        return Token::KEY_F18;
    case '9':
        return Token::KEY_F19;
    default:
        LOG( INFO ) << "Unknown function key number: " << character;
        return std::nullopt;
    }
}

std::optional<Token> checkIfLegalCapitalLiteral( std::string input ) noexcept
{
    // C++ can't do string comparisons in switches. :(
    if ( input == "BACKSPACE" )
    {
        return Token::KEY_BACKSPACE;
    }
    if ( input == "SPACE" )
    {
        return Token::KEY_SPACE;
    }
    if ( input == "TAB" )
    {
        return Token::KEY_TAB;
    }
    if ( input == "ESC" )
    {
        return Token::KEY_ESC;
    }
    if ( input == "INS" )
    {
        return Token::KEY_INS;
    }
    if ( input == "DEL" )
    {
        return Token::KEY_DEL;
    }
    if ( input == "END" )
    {
        return Token::KEY_END;
    }
    if ( input == "PGDN" )
    {
        return Token::KEY_PGDN;
    }
    if ( input == "PGUP" )
    {
        return Token::KEY_PGUP;
    }
    if ( input == "CAPS" )
    {
        return Token::KEY_CAPS;
    }
    if ( input == "PRNSCRN" )
    {
        return Token::KEY_PRNSCRN;
    }
    if ( input == "PAUSE" )
    {
        return Token::KEY_PAUSE;
    }
    if ( input == "SCRLOCK" )
    {
        return Token::KEY_SCRLOCK;
    }
    if ( input == "LEFTARROW" )
    {
        return Token::KEY_LEFTARROW;
    }
    if ( input == "RIGHTARROW" )
    {
        return Token::KEY_RIGHTARROW;
    }
    if ( input == "UPARROW" )
    {
        return Token::KEY_UPARROW;
    }
    if ( input == "DOWNARROW" )
    {
        return Token::KEY_DOWNARROW;
    }
    if ( input == "KPSLASH" )
    {
        return Token::KEY_KPSLASH;
    }
    if ( input == "KPSTAR" )
    {
        return Token::KEY_KPSTAR;
    }
    if ( input == "KPMINUS" )
    {
        return Token::KEY_KPMINUS;
    }
    if ( input == "KPPLUS" )
    {
        return Token::KEY_KPPLUS;
    }
    if ( input == "ENTER" )
    {
        return Token::KEY_ENTER;
    }
    if ( input == "BACKSLASH" )
    {
        return Token::KEY_BACKSLASH;
    }

    return std::nullopt;
}

std::vector<Token>
    ParseKeyboardInputsToTokens( const std::string inputs ) noexcept
{
    std::vector<Token> tokens{};

    for ( auto ch = inputs.begin(), end = inputs.end(); ch != end; ++ch )
    {
        if ( const auto c = getNumberOrLetter( *ch ); c.has_value() )
        {
            tokens.push_back( *c );
            continue;
        }
        if ( const auto c = getModifier( *ch ); c.has_value() )
        {
            tokens.push_back( Token::TOKEN_NO_KEYUP_NEXT );
            tokens.push_back( *c );
            continue;
        }

        if ( isspace( *ch ) )
        {
            tokens.push_back( Token::TOKEN_NEW_SEQUENCE );
        }

        if ( !isupper( *ch ) || isspace( *ch ) )
        {
            LOG( INFO ) << "Unknown character found in sequence: " << *ch;
            continue;
        }
        // Everything below is upper case

        if ( *ch == 'F' )
        {
            ++ch;
            if ( ch == end )
            {
                break;
            }
            const auto token = getFunctionNumber( *ch );
            if ( token.has_value() )
            {
                tokens.push_back( *token );
                continue;
            }
            else
            {
                // Spec says to abort on errors and submit correct values before
                // error.
                break;
            }
        }
        else if ( *ch == 'G' )
        {
            ++ch;
            if ( ch == end )
            {
                break;
            }
            const auto token = getFunctionNumberExtended( *ch );
            if ( token.has_value() )
            {
                tokens.push_back( *token );
                continue;
            }
            else
            {
                // Spec says to abort on errors and submit correct values before
                // error.
                break;
            }
        }

        std::string characters;
        characters.push_back( *ch );
        while ( ch + 1 != end )
        {
            ++ch;
            characters.push_back( *ch );
            if ( auto c = checkIfLegalCapitalLiteral( characters );
                 c.has_value() )
            {
                tokens.push_back( *c );
                characters.clear();
                // break out of the while
                break;
            }
        }
        if ( ch == end )
        {
            break;
        }
    }

    return tokens;
}

std::vector<Token>
    removeDuplicateNewSequences( const std::vector<Token>& tokens ) noexcept
{
    std::vector<Token> approvedTokens = {};

    bool previouslyNewSequenceToken = false;
    for ( const auto& token : tokens )
    {
        if ( token != Token::TOKEN_NEW_SEQUENCE )
        {
            previouslyNewSequenceToken = false;
        }

        if ( previouslyNewSequenceToken )
        {
            continue;
        }

        if ( token == Token::TOKEN_NEW_SEQUENCE )
        {
            previouslyNewSequenceToken = true;
        }

        approvedTokens.push_back( token );
    }

    return approvedTokens;
}

std::vector<Token>
    removeTokenInSameSequence( const std::vector<Token>& tokens,
                               const Token tokenToRemove ) noexcept
{
    std::vector<Token> approvedTokens = {};

    bool tokenInSequence = false;
    for ( const auto& token : tokens )
    {
        if ( token == Token::TOKEN_NEW_SEQUENCE )
        {
            tokenInSequence = false;
        }

        if ( tokenInSequence && ( token == tokenToRemove ) )
        {
            continue;
        }

        if ( token == tokenToRemove )
        {
            tokenInSequence = true;
        }
        approvedTokens.push_back( token );
    }
    return approvedTokens;
}

std::vector<Token>
    removeIncorrectTokens( const std::vector<Token>& tokens ) noexcept
{
    const auto spacesRemoved = removeDuplicateNewSequences( tokens );
    const auto ctrlRemoved
        = removeTokenInSameSequence( spacesRemoved, Token::MODIFIER_CTRL );
    const auto altRemoved
        = removeTokenInSameSequence( ctrlRemoved, Token::MODIFIER_ALT );
    const auto shiftRemoved
        = removeTokenInSameSequence( altRemoved, Token::MODIFIER_SHIFT );
    const auto superRemoved
        = removeTokenInSameSequence( shiftRemoved, Token::MODIFIER_SUPER );

    return superRemoved;
}

bool isModifier( const Token token ) noexcept
{
    switch ( token )
    {
    case Token::MODIFIER_CTRL:
        [[fallthrough]];
    case Token::MODIFIER_ALT:
        [[fallthrough]];
    case Token::MODIFIER_SHIFT:
        [[fallthrough]];
    case Token::MODIFIER_RSHIFT:
        [[fallthrough]];
    case Token::MODIFIER_SUPER:
        [[fallthrough]];
    case Token::MODIFIER_TILDE:
        return true;
    default:
        return false;
    }
}

bool isLiteralKey( const Token token ) noexcept
{
    switch ( token )
    {
    case Token::KEY_a:
        return true;
    case Token::KEY_b:
        return true;
    case Token::KEY_c:
        return true;
    case Token::KEY_d:
        return true;
    case Token::KEY_e:
        return true;
    case Token::KEY_f:
        return true;
    case Token::KEY_g:
        return true;
    case Token::KEY_h:
        return true;
    case Token::KEY_i:
        return true;
    case Token::KEY_j:
        return true;
    case Token::KEY_k:
        return true;
    case Token::KEY_l:
        return true;
    case Token::KEY_m:
        return true;
    case Token::KEY_n:
        return true;
    case Token::KEY_o:
        return true;
    case Token::KEY_p:
        return true;
    case Token::KEY_q:
        return true;
    case Token::KEY_r:
        return true;
    case Token::KEY_s:
        return true;
    case Token::KEY_t:
        return true;
    case Token::KEY_u:
        return true;
    case Token::KEY_v:
        return true;
    case Token::KEY_w:
        return true;
    case Token::KEY_x:
        return true;
    case Token::KEY_y:
        return true;
    case Token::KEY_z:
        return true;

    case Token::KEY_0:
        return true;
    case Token::KEY_1:
        return true;
    case Token::KEY_2:
        return true;
    case Token::KEY_3:
        return true;
    case Token::KEY_4:
        return true;
    case Token::KEY_5:
        return true;
    case Token::KEY_6:
        return true;
    case Token::KEY_7:
        return true;
    case Token::KEY_8:
        return true;
    case Token::KEY_9:
        return true;

    default:
        return false;
    }
}

bool isLiteral( const Token token ) noexcept
{
    if ( isLiteralKey( token ) )
    {
        return true;
    }
    switch ( token )
    {
    case Token::KEY_F1:
        return true;
    case Token::KEY_F2:
        return true;
    case Token::KEY_F3:
        return true;
    case Token::KEY_F4:
        return true;
    case Token::KEY_F5:
        return true;
    case Token::KEY_F6:
        return true;
    case Token::KEY_F7:
        return true;
    case Token::KEY_F8:
        return true;
    case Token::KEY_F9:
        return true;
    case Token::KEY_F10:
        return true;
    case Token::KEY_F11:
        return true;
    case Token::KEY_F12:
        return true;
    case Token::KEY_F13:
        return true;
    case Token::KEY_F14:
        return true;
    case Token::KEY_F15:
        return true;
    case Token::KEY_F16:
        return true;
    case Token::KEY_F17:
        return true;
    case Token::KEY_F18:
        return true;
    case Token::KEY_F19:
        return true;

    case Token::KEY_BACKSPACE:
        return true;
    case Token::KEY_SPACE:
        return true;
    case Token::KEY_TAB:
        return true;
    case Token::KEY_ESC:
        return true;
    case Token::KEY_INS:
        return true;
    case Token::KEY_DEL:
        return true;
    case Token::KEY_END:
        return true;
    case Token::KEY_PGDN:
        return true;
    case Token::KEY_PGUP:
        return true;
    case Token::KEY_CAPS:
        return true;
    case Token::KEY_PRNSCRN:
        return true;
    case Token::KEY_PAUSE:
        return true;
    case Token::KEY_SCRLOCK:
        return true;
    case Token::KEY_LEFTARROW:
        return true;
    case Token::KEY_RIGHTARROW:
        return true;
    case Token::KEY_UPARROW:
        return true;
    case Token::KEY_DOWNARROW:
        return true;
    case Token::KEY_KPSLASH:
        return true;
    case Token::KEY_KPSTAR:
        return true;
    case Token::KEY_KPMINUS:
        return true;
    case Token::KEY_KPPLUS:
        return true;
    case Token::KEY_ENTER:
        return true;
    case Token::KEY_BACKSLASH:
        return true;

    default:
        return false;
    }
}
