#-------------------------------------------------
#
# Project created by QtCreator 2015-06-10T16:57:45
#
#-------------------------------------------------

QT       += core gui qml quick multimedia widgets
CONFIG   += c++1z

DEFINES += ELPP_THREAD_SAFE ELPP_QT_LOGGING ELPP_NO_DEFAULT_LOG_FILE

lessThan(QT_MAJOR_VERSION, 5): error("requires Qt 5.6 or higher")
lessThan(QT_MINOR_VERSION, 6): error("requires Qt 5.6 or higher")

TARGET = AdvancedSettings
TEMPLATE = app

include_dir = $$PWD/build_scripts/qt/

win32-msvc {
    include($$include_dir/compilers/msvc.pri)
}

win32-clang-msvc{
    include($$include_dir/compilers/clang-msvc.pri)
}

*g++* {
    include($$include_dir/compilers/gcc.pri)
}

#Look for anything clang that is not clang-msvc, since it does not
#allow all the same switches as regular clang.
*clang|*clang-g++|*clang-libc++ {
    include($$include_dir/compilers/clang.pri)
}

SOURCES += src/main.cpp\
    src/overlaycontroller.cpp \
    src/tabcontrollers/AudioTabController.cpp \
    src/tabcontrollers/ChaperoneTabController.cpp \
    src/tabcontrollers/FixFloorTabController.cpp \
    src/tabcontrollers/MoveCenterTabController.cpp \
    src/tabcontrollers/SettingsTabController.cpp \
    src/tabcontrollers/StatisticsTabController.cpp \
    src/tabcontrollers/SteamVRTabController.cpp \
    src/tabcontrollers/ReviveTabController.cpp \
    src/tabcontrollers/UtilitiesTabController.cpp \
    src/tabcontrollers/PttController.cpp \
    src/utils/ChaperoneUtils.cpp \
    src/tabcontrollers/audiomanager/AudioManagerDummy.cpp \
    src/tabcontrollers/keyboardinput/KeyboardInputDummy.cpp \
    src/overlaycontroller/openvr_init.cpp \
    src/ivrinput/ivrinput.cpp

HEADERS += src/overlaycontroller.h \
    src/tabcontrollers/AudioTabController.h \
    src/tabcontrollers/ChaperoneTabController.h \
    src/tabcontrollers/FixFloorTabController.h \
    src/tabcontrollers/MoveCenterTabController.h \
    src/tabcontrollers/SettingsTabController.h \
    src/tabcontrollers/StatisticsTabController.h \
    src/tabcontrollers/SteamVRTabController.h \
    src/tabcontrollers/ReviveTabController.h \
    src/tabcontrollers/UtilitiesTabController.h \
    src/tabcontrollers/AudioManager.h \
    src/tabcontrollers/PttController.h \
    src/tabcontrollers/KeyboardInput.h \
    src/utils/Matrix.h \
    src/utils/ChaperoneUtils.h \
    src/tabcontrollers/audiomanager/AudioManagerDummy.h \
    src/tabcontrollers/keyboardinput/KeyboardInputDummy.h \
    src/overlaycontroller/openvr_init.h \
    src/ivrinput/ivrinput_action.h \
    src/ivrinput/ivrinput_manifest.h \
    src/ivrinput/ivrinput_action_set.h \
    src/ivrinput/ivrinput.h

win32 {
    SOURCES += src/tabcontrollers/audiomanager/AudioManagerWindows.cpp \
        src/tabcontrollers/keyboardinput/KeyboardInputWindows.cpp
    HEADERS += src/tabcontrollers/audiomanager/AudioManagerWindows.h \
		src/tabcontrollers/audiomanager/IPolicyConfig.h \
        src/tabcontrollers/keyboardinput/KeyboardInputWindows.h
}

INCLUDEPATH += third-party/openvr/headers \
                        third-party/easylogging++
#easylogging++ used to be a header only lib. Now requires easylogging++.cc
SOURCES += third-party/easylogging++/easylogging++.cc
win32:LIBS += -L"$$PWD/third-party/openvr/lib/win64" -luser32 -lole32
unix:LIBS += -L"$$PWD/third-party/openvr/lib/linux64"
LIBS += -lopenvr_api

win32:DESTDIR = bin/win64/AdvancedSettings
unix:DESTDIR = bin/linux/AdvancedSettings

DISTFILES += \
    src/res/sounds/alarm01.wav \
    src/res/img/audio/microphone/ptt_notification.png \
    src/res/img/audio/microphone/ptt_notification.svg \
    src/res/img/battery/* \
    src/res/img/icons/* \
    src/res/qml/qmldir \
    src/res/qml/audio_page/* \
    src/res/qml/ChaperonePage.qml \
    src/res/qml/ChaperoneWarningsPage.qml \
    src/res/qml/FixFloorPage.qml \
    src/res/qml/PlayspacePage.qml \
    src/res/qml/RevivePage.qml \
    src/res/qml/RootPage.qml \
    src/res/qml/SettingsPage.qml \
    src/res/qml/StatisticsPage.qml \
    src/res/qml/SteamVRPage.qml \
    src/res/qml/utilities_page/* \
    src/res/qml/utilities_page/media_keys/* \
    src/res/qml/utilities_page/keyboard_utils/* \
    src/res/qml/utilities_page/desktop_size/* \
    src/res/qml/utilities_page/alarm_clock/* \
    src/res/qml/audio_page/dialog_boxes/* \
    src/res/qml/audio_page/device_selector/* \
    src/res/qml/audio_page/proximity/* \
    src/res/qml/audio_page/push_to_talk/* \
    src/res/qml/audio_page/profiles/* \
    src/res/qml/common/* \

RESOURCES += src/res/resources.qrc
