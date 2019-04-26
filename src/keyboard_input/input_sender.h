#pragma once
#include "input_parser.h"
#include <easylogging++.h>

enum class KeyStatus
{
    Up,
    Down,
};

void sendTokensAsInput( const std::vector<Token> tokens );
