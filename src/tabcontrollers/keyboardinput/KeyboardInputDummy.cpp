#include "KeyboardInputDummy.h"

// Used to get the compiler to shut up about C4100: unreferenced formal parameter.
// The cast is to get GCC to shut up about it.
#define UNREFERENCED_PARAMETER(P) static_cast<void>((P))

namespace advsettings {

void KeyboardInputDummy::sendKeyboardInput(QString input)
{
    // noop
    UNREFERENCED_PARAMETER(input);
}

void KeyboardInputDummy::sendKeyboardEnter()
{
    // noop
}

void KeyboardInputDummy::sendKeyboardBackspace(int count)
{
    // noop
    UNREFERENCED_PARAMETER(count);
}

void KeyboardInputDummy::sendKeyboardAltTab()
{
    // noop
}

}
