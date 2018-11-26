#pragma once

#include <vector>
#include <QString>

// application namespace
namespace advsettings {

class AudioTabController;

class KeyboardInput
{
public:
    virtual ~KeyboardInput() {};

    virtual void sendKeyboardInput(QString input) = 0;
    virtual void sendKeyboardEnter() = 0;
    virtual void sendKeyboardBackspace(int count) = 0;
};

}

