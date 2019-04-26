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
        // LOG incorrect character
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

    // LOG incorrect character
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
    case Token::MODIFIER_SUPER:
        return true;
    default:
        return false;
    }
}

bool isLiteral( const Token token ) noexcept
{
    if ( isalnum( static_cast<int>( token ) ) )
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

    default:
        return false;
    }
}
