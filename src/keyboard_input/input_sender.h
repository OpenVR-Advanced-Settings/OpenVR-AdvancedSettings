#pragma once
#include "input_parser.h"
#include <easylogging++.h>

enum class KeyStatus
{
    Up,
    Down,
};

void sendTokensAsInput( const std::vector<Token> tokens );

void sendStringAsInput( const std::string input )
{
    const auto tokens = ParseKeyboardInputsToTokens( input );
    const auto inputs = removeIncorrectTokens( tokens );
    sendTokensAsInput( inputs );
}
