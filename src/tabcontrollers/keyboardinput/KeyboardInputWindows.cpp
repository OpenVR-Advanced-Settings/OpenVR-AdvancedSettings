#include "KeyboardInputWindows.h"
#include <Windows.h>

namespace advsettings {

void fillKiStruct(INPUT& ip, WORD scanCode, bool keyup) {
    ip.type = INPUT_KEYBOARD;
    ip.ki.wVk = scanCode;
    if (keyup) {
        ip.ki.dwFlags = KEYEVENTF_KEYUP;
    } else {
        ip.ki.dwFlags = 0;
    }
    ip.ki.wScan = 0;
    ip.ki.dwExtraInfo = 0;
    ip.ki.time = 0;
};

void KeyboardInputWindows::sendKeyboardInput(QString input)
{
    int len = input.length();
    if (len > 0) {
        LPINPUT ips = new INPUT[len * 5 + 3];
        unsigned ii = 0;
        bool shiftPressed = false;
        bool ctrlPressed = false;
        bool altPressed = false;
        for (int i = 0; i < len; i++) {
            short tmp = VkKeyScan(input.at(i).toLatin1());
            WORD c = tmp & 0xFF;
            short shiftState = tmp >> 8;
            bool isShift = shiftState & 1;
            bool isCtrl = shiftState & 2;
            bool isAlt = shiftState & 4;
            if (isShift && !shiftPressed) {
                fillKiStruct(ips[ii++], VK_SHIFT, false);
                shiftPressed = true;
            } else if (!isShift && shiftPressed) {
                fillKiStruct(ips[ii++], VK_SHIFT, true);
                shiftPressed = false;
            }
            if (isCtrl && !ctrlPressed) {
                fillKiStruct(ips[ii++], VK_CONTROL, false);
                ctrlPressed = true;
            } else if (!isCtrl && ctrlPressed) {
                fillKiStruct(ips[ii++], VK_CONTROL, true);
                ctrlPressed = false;
            }
            if (isAlt && !altPressed) {
                fillKiStruct(ips[ii++], VK_MENU, false);
                altPressed = true;
            } else if (!isAlt && altPressed) {
                fillKiStruct(ips[ii++], VK_MENU, true);
                altPressed = false;
            }
            fillKiStruct(ips[ii++], c, false);
            fillKiStruct(ips[ii++], c, true);
        }
        if (shiftPressed) {
            fillKiStruct(ips[ii++], VK_SHIFT, true);
            shiftPressed = false;
        }
        if (ctrlPressed) {
            fillKiStruct(ips[ii++], VK_CONTROL, true);
            ctrlPressed = false;
        }
        if (altPressed) {
            fillKiStruct(ips[ii++], VK_MENU, true);
            altPressed = false;
        }
        if (!SendInput(ii, ips, sizeof(INPUT))) {
            char *err;
            auto errCode = GetLastError();
            if (!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                    NULL,
                    errCode,
                    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // default language
                    (LPTSTR)&err,
                    0,
                    NULL)){
                LOG(ERROR) << "Error calling SendInput(): Could not get error message (" << errCode << ")";
            } else {
                LOG(ERROR) << "Error calling SendInput(): " << err;
            }
        }
        delete ips;
    }
}

void KeyboardInputWindows::sendKeyboardEnter()
{
    LPINPUT ips = new INPUT[2];
    fillKiStruct(ips[0], VK_RETURN, false);
    fillKiStruct(ips[1], VK_RETURN, true);
    if (!SendInput(2, ips, sizeof(INPUT))) {
        char *err;
        auto errCode = GetLastError();
        if (!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
            NULL,
            errCode,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // default language
            (LPTSTR)&err,
            0,
            NULL)) {
            LOG(ERROR) << "Error calling SendInput(): Could not get error message (" << errCode << ")";
        } else {
            LOG(ERROR) << "Error calling SendInput(): " << err;
        }
    }
    delete ips;
}

void KeyboardInputWindows::sendKeyboardBackspace(int count)
{
    if (count > 0) {
        LPINPUT ips = new INPUT[count * 2];
        for (int i = 0; i < count; i++) {
            fillKiStruct(ips[2 * i], VK_BACK, false);
            fillKiStruct(ips[2 * i + 1], VK_BACK, true);
        }
        if (!SendInput(2 * count, ips, sizeof(INPUT))) {
            char *err;
            auto errCode = GetLastError();
            if (!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                NULL,
                errCode,
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // default language
                (LPTSTR)&err,
                0,
                NULL)) {
                LOG(ERROR) << "Error calling SendInput(): Could not get error message (" << errCode << ")";
            } else {
                LOG(ERROR) << "Error calling SendInput(): " << err;
            }
        }
        delete ips;
    }
}

}
