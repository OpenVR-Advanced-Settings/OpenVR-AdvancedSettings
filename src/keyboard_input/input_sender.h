#pragma once
#include "input_parser.h"
#include <easylogging++.h>

enum class KeyStatus
{
    Up,
    Down,
};

void initOsSystems();
void shutdownOsSystems();

void sendKeyPress( const Token token, const KeyStatus status );

inline void sendTokensAsInput( const std::vector<Token> tokens )
{
    initOsSystems();

    std::vector<Token> heldInputs = {};
    bool noKeyUp = false;
    for ( const auto& token : tokens )
    {
        if ( token == Token::TOKEN_NO_KEYUP_NEXT )
        {
            noKeyUp = true;
            continue;
        }

        if ( isModifier( token ) )
        {
            sendKeyPress( token, KeyStatus::Down );
            if ( !noKeyUp )
            {
                heldInputs.push_back( token );
            }
            continue;
        }

        if ( token == Token::TOKEN_NEW_SEQUENCE )
        {
            for ( const auto& h : heldInputs )
            {
                sendKeyPress( h, KeyStatus::Up );
            }
            heldInputs.clear();
            continue;
        }

        if ( isLiteral( token ) )
        {
            sendKeyPress( token, KeyStatus::Down );
            if ( !noKeyUp )
            {
                sendKeyPress( token, KeyStatus::Up );
            }
            continue;
        }

        if ( token != Token::TOKEN_NO_KEYUP_NEXT )
        {
            noKeyUp = false;
            continue;
        }
    }

    if ( !heldInputs.empty() )
    {
        for ( const auto& h : heldInputs )
        {
            sendKeyPress( h, KeyStatus::Up );
        }
    }

    shutdownOsSystems();
}

inline void sendStringAsInput( const std::string input )
{
    const auto tokens = ParseKeyboardInputsToTokens( input );
    const auto inputs = removeIncorrectTokens( tokens );
    sendTokensAsInput( inputs );
}
