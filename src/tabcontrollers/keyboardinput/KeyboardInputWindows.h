#ifndef KEYBOARDINPUTWINDOWS_H
#define KEYBOARDINPUTWINDOWS_H

#include "../KeyboardInput.h"

// application namespace
namespace advsettings
{
class KeyboardInputWindows : public KeyboardInput
{
public:
    virtual void sendKeyboardInput( QString input ) override;
    virtual void sendKeyboardEnter() override;
    virtual void sendKeyboardBackspace( const int count ) override;
    virtual void sendKeyboardAltTab() override;
};

} // namespace advsettings

#endif // KEYBOARDINPUTWINDOWS_H
