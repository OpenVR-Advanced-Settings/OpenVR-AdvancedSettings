#include "media_keys.h"
#include "src/keyboard_input/input_sender.h"
#include <Windows.h>

namespace keyboardinput
{
INPUT createInputStruct( const WORD virtualKeyCode,
                         const KeyStatus keyStatus ) noexcept
{
    INPUT input = {};

    input.type = INPUT_KEYBOARD;

    input.ki.wVk = virtualKeyCode;

    if ( keyStatus == KeyStatus::Up )
    {
        input.ki.dwFlags = KEYEVENTF_KEYUP;
    }
    else if ( keyStatus == KeyStatus::Down )
    {
        input.ki.dwFlags = 0;
    }

    return input;
}

void sendKeyboardInputRaw( std::vector<INPUT> inputs )
{
    const auto success = SendInput( static_cast<unsigned int>( inputs.size() ),
                                    inputs.data(),
                                    sizeof( INPUT ) );

    if ( ( inputs.size() > 0 ) && !success )
    {
        char* err;
        auto errCode = GetLastError();
        if ( !FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER
                                 | FORMAT_MESSAGE_FROM_SYSTEM,
                             nullptr,
                             errCode,
                             MAKELANGID( LANG_NEUTRAL,
                                         SUBLANG_DEFAULT ), // default language
                             reinterpret_cast<LPTSTR>( &err ),
                             0,
                             nullptr ) )
        {
            LOG( ERROR )
                << "Error calling SendInput(): Could not get error message ("
                << errCode << ")";
        }
        else
        {
            LOG( ERROR ) << "Error calling SendInput(): " << err;
        }
    }
}

void sendMediaNextSong()
{
    std::vector<INPUT> inputs = {};
    inputs.push_back(
        createInputStruct( VK_MEDIA_NEXT_TRACK, KeyStatus::Down ) );
    inputs.push_back( createInputStruct( VK_MEDIA_NEXT_TRACK, KeyStatus::Up ) );
    sendKeyboardInputRaw( inputs );
}

void sendMediaPreviousSong()
{
    std::vector<INPUT> inputs = {};
    inputs.push_back(
        createInputStruct( VK_MEDIA_PREV_TRACK, KeyStatus::Down ) );
    inputs.push_back( createInputStruct( VK_MEDIA_PREV_TRACK, KeyStatus::Up ) );
    sendKeyboardInputRaw( inputs );
}

void sendMediaPausePlay()
{
    std::vector<INPUT> inputs = {};
    inputs.push_back(
        createInputStruct( VK_MEDIA_PLAY_PAUSE, KeyStatus::Down ) );
    inputs.push_back( createInputStruct( VK_MEDIA_PLAY_PAUSE, KeyStatus::Up ) );
    sendKeyboardInputRaw( inputs );
}

void sendMediaStopSong()
{
    std::vector<INPUT> inputs = {};
    inputs.push_back( createInputStruct( VK_MEDIA_STOP, KeyStatus::Down ) );
    inputs.push_back( createInputStruct( VK_MEDIA_STOP, KeyStatus::Up ) );
    sendKeyboardInputRaw( inputs ); // noop
}

} // namespace keyboardinput
