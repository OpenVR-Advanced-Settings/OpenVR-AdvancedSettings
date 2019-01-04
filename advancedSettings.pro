#-------------------------------------------------
#
# Project created by QtCreator 2015-06-10T16:57:45
#
#-------------------------------------------------

QT       += core gui qml quick multimedia widgets
CONFIG   += c++14

DEFINES += ELPP_THREAD_SAFE ELPP_QT_LOGGING ELPP_NO_DEFAULT_LOG_FILE

lessThan(QT_MAJOR_VERSION, 5): error("requires Qt 5.6 or higher")
lessThan(QT_MINOR_VERSION, 6): error("requires Qt 5.6 or higher")

TARGET = AdvancedSettings
TEMPLATE = app

*msvc* {
    #Removing -W3 from both FLAGS is necessary, otherwise compiler will give
    #D9025: overriding '/W4' with '/W3'
    QMAKE_CFLAGS_WARN_ON -= -W3
    QMAKE_CXXFLAGS_WARN_ON -= -W3
    #C4127 was in a third party file with no way to turn off.
    QMAKE_CXXFLAGS += /W4 /wd4127
    QMAKE_CXXFLAGS += /WX

}

*g++* {
    QMAKE_CXXFLAGS += -Werror
}

#Look for anything clang that is not clang-msvc, since it does not
#allow all the same switches as regular clang.
*clang|*clang-g++|*clang-libc++ {
    QMAKE_CXXFLAGS += -Werror
    #All includes from the third-party directory will not warn.
    QMAKE_CXXFLAGS += --system-header-prefix=third-party
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
    src/tabcontrollers/AccessibilityTabController.cpp \
    src/utils/ChaperoneUtils.cpp \
    src/tabcontrollers/audiomanager/AudioManagerDummy.cpp \
    src/tabcontrollers/keyboardinput/KeyboardInputDummy.cpp

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
    src/tabcontrollers/AccessibilityTabController.h \
    src/tabcontrollers/KeyboardInput.h \
    src/utils/Matrix.h \
    src/utils/ChaperoneUtils.h \
    src/tabcontrollers/audiomanager/AudioManagerDummy.h \
    src/tabcontrollers/keyboardinput/KeyboardInputDummy.h

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
    src/res/qml/ptt_notification.png \
    src/res/battery_0.png \
    src/res/battery_1.png \
    src/res/battery_2.png \
    src/res/battery_3.png \
    src/res/battery_4.png \
    src/res/battery_5.png \
    src/res/thumbicon.png \
    src/res/qml/backarrow.svg \
    src/res/qml/check.svg \
    src/res/qml/mic_off.svg \
    src/res/qml/mic_on.svg \
    src/res/qml/ptt_notification.svg \
    src/res/qml/speaker_off.svg \
    src/res/qml/speaker_on.svg \
    src/res/qml/qmldir \
    src/res/qml/AccessibilityPage.qml \
    src/res/qml/AudioPage.qml \
    src/res/qml/ChaperonePage.qml \
    src/res/qml/ChaperoneWarningsPage.qml \
    src/res/qml/FixFloorPage.qml \
    src/res/qml/mainwidget.qml \
    src/res/qml/MyComboBox.qml \
    src/res/qml/MyDialogOkCancelPopup.qml \
    src/res/qml/MyDialogOkPopup.qml \
    src/res/qml/MyPushButton.qml \
    src/res/qml/MyPushButton2.qml \
    src/res/qml/MyResources.qml \
    src/res/qml/MySlider.qml \
    src/res/qml/MyStackViewPage.qml \
    src/res/qml/MyText.qml \
    src/res/qml/MyTextField.qml \
    src/res/qml/MyToggleButton.qml \
    src/res/qml/PlayspacePage.qml \
    src/res/qml/PttControllerConfigDialog.qml \
    src/res/qml/RevivePage.qml \
    src/res/qml/RootPage.qml \
    src/res/qml/SettingsPage.qml \
    src/res/qml/StatisticsPage.qml \
    src/res/qml/SteamVRPage.qml \
    src/res/qml/UtilitiesPage.qml
