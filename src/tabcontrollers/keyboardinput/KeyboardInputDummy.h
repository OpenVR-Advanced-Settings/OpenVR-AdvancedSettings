#ifndef KEYBOARDINPUTDUMMY_H
#define KEYBOARDINPUTDUMMY_H

#include "../KeyboardInput.h"

// application namespace
namespace advsettings {

class KeyboardInputDummy : public KeyboardInput
{
public:
    virtual void sendKeyboardInput(QString input) override;
    virtual void sendKeyboardEnter() override;
    virtual void sendKeyboardBackspace(int count) override;
    virtual void sendKeyboardAltTab() override;
};

}

#endif // KEYBOARDINPUTDUMMY_H
