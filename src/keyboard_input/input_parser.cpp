#include "input_parser.h"

std::optional<Token> getModifier( const char& character )
{
    switch ( character )
    {
    case '^':
        return Token::MODIFIER_CTRL;
    case '*':
        return Token::MODIFIER_ALT;
    case '>':
        return Token::MODIFIER_SHIFT;
    case '/':
        return Token::MODIFIER_ALTGR;
    case '#':
        return Token::MODIFIER_SUPER;
    default:
        return std::nullopt;
    }
}

std::optional<Token> getNumberOrLetter( const char& character )
{
    if ( islower( character ) || isdigit( character ) )
    {
        return static_cast<Token>( character );
    }
    return std::nullopt;
}

std::optional<Token> getFunctionNumber( const char& character )
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
        // LOG incorrect character
        return std::nullopt;
    }
}

std::optional<Token> checkIfLegalCapitalLiteral( std::string input )
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

    // LOG incorrect character
    return std::nullopt;
}

std::vector<Token> ParseKeyboardInputsToTokens( const std::string inputs )
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
            tokens.push_back( *c );
            continue;
        }

        if ( isspace( *ch ) )
        {
            tokens.push_back( Token::TOKEN_NEW_SEQUENCE );
        }

        if ( !isupper( *ch ) || isspace( *ch ) )
        {
            // LOG incorrect character
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

        std::string characters = "";
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
