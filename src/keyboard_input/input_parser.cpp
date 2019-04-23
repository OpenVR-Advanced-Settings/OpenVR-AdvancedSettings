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

std::vector<Token> ParseKeyboardInputsToTokens( const std::string inputs )
{
    std::vector<Token> tokens{};

    for ( auto ch = inputs.begin(); ch != inputs.end(); ++ch )
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

        if ( !isupper( *ch ) )
        {
            // LOG incorrect character
            continue;
        }
        // Everything below is upper case

        if ( *ch == 'F' )
        {
            ++ch;
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
    }

    return tokens;
}
