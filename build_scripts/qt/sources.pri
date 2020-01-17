SOURCES += src/main.cpp\
    src/overlaycontroller.cpp \
    src/tabcontrollers/AudioTabController.cpp \
    src/tabcontrollers/ChaperoneTabController.cpp \
    src/tabcontrollers/FixFloorTabController.cpp \
    src/tabcontrollers/MoveCenterTabController.cpp \
    src/tabcontrollers/SettingsTabController.cpp \
    src/tabcontrollers/StatisticsTabController.cpp \
    src/tabcontrollers/SteamVRTabController.cpp \
    src/tabcontrollers/UtilitiesTabController.cpp \
    src/tabcontrollers/VideoTabController.cpp \
    src/utils/ChaperoneUtils.cpp \
    src/openvr/openvr_init.cpp \
    src/openvr/ivrinput.cpp \
    src/utils/setup.cpp \
    src/utils/paths.cpp \
    src/utils/FrameRateUtils.cpp \
    src/keyboard_input/keyboard_input.cpp \
    src/keyboard_input/input_parser.cpp \
    src/settings/settings.cpp \
    src/settings/settings_object.cpp \


HEADERS += src/overlaycontroller.h \
    src/tabcontrollers/AudioTabController.h \
    src/tabcontrollers/ChaperoneTabController.h \
    src/tabcontrollers/FixFloorTabController.h \
    src/tabcontrollers/MoveCenterTabController.h \
    src/tabcontrollers/SettingsTabController.h \
    src/tabcontrollers/StatisticsTabController.h \
    src/tabcontrollers/SteamVRTabController.h \
    src/tabcontrollers/UtilitiesTabController.h \
    src/tabcontrollers/VideoTabController.h \
    src/tabcontrollers/audiomanager/AudioManager.h \
    src/keyboard_input/keyboard_input.h \
    src/media_keys/media_keys.h \
    src/utils/Matrix.h \
    src/utils/ChaperoneUtils.h \
    src/quaternion/quaternion.h \
    src/openvr/openvr_init.h \
    src/openvr/ivrinput_action.h \
    src/openvr/ivrinput_manifest.h \
    src/openvr/ivrinput_action_set.h \
    src/openvr/ivrinput_input_source.h \
    src/openvr/ivrinput.h \
    src/utils/setup.h \
    src/utils/paths.h \
    src/utils/FrameRateUtils.h \
    src/keyboard_input/input_parser.h \
    src/keyboard_input/input_sender.h \
    src/settings/settings.h \
    src/settings/internal/setting_value.h \
    src/settings/internal/settings_internal.h \
    src/settings/internal/settings_controller.h \
    src/settings/internal/specific_setting_value.h \
    src/settings/settings_object.h \
    src/settings/internal/settings_object_data.h

win32 {
    SOURCES += src/tabcontrollers/audiomanager/AudioManagerWindows.cpp \
        src/keyboard_input/input_sender_win.cpp \
        src/media_keys/media_keys_win.cpp
    HEADERS += src/tabcontrollers/audiomanager/AudioManagerWindows.h
}

unix:!macx {
    !noX11 {
        message(X11 features enabled.)
        SOURCES += src/keyboard_input/input_sender_X11.cpp
        CONFIG += x11
        LIBS += -lXtst
    }
    else {
        message(X11 features disabled.)
        SOURCES += src/keyboard_input/input_sender_dummy.cpp
    }

    !noDBUS {
        message(DBUS features enabled.)
        SOURCES += src/media_keys/media_keys_dbus.cpp
        QT += dbus
    }
    else {
        message(DBUS features disabled.)
        SOURCES += src/media_keys/media_keys_dummy.cpp
    }

    !noPulse {
        message(PulseAudio features enabled.)
        SOURCES += src/tabcontrollers/audiomanager/AudioManagerPulse.cpp
        HEADERS += src/tabcontrollers/audiomanager/AudioManagerPulse.h \
            src/tabcontrollers/audiomanager/AudioManagerPulse_internal.h
        LIBS += -lpulse
    }
    else {
        message(PulseAudio features disabled.)
        SOURCES += src/tabcontrollers/audiomanager/AudioManagerDummy.cpp
        HEADERS += src/tabcontrollers/audiomanager/AudioManagerDummy.h
    }
}

macx {
    SOURCES += src/keyboard_input/input_sender_dummy.cpp \
                src/media_keys/media_keys_dummy.cpp \
                src/tabcontrollers/audiomanager/AudioManagerDummy.cpp
    HEADERS += src/tabcontrollers/audiomanager/AudioManagerDummy.h

}

win32-msvc {
    INCLUDEPATH += third-party/openvr/headers \
                    third-party/easylogging++ \
                    third-party/policyconfig
}

win32-clang-msvc {
    QMAKE_CXXFLAGS += /imsvc "$$PWD/../../third-party/openvr/headers"
    QMAKE_CXXFLAGS += /imsvc "$$PWD/../../third-party/easylogging++"
    QMAKE_CXXFLAGS += /imsvc "$$PWD/../../third-party/policyconfig"
}

# Anything g++ or clang
# In order to suppress warnings in third party headers
*clang|*clang-g++|*clang-libc++|*g++* {
    #Force Linux to look into the current dir for the OpenVR lib
    QMAKE_LFLAGS    += '-Wl,-rpath,\'\$$ORIGIN\''

    QMAKE_CXXFLAGS += -isystem $$[QT_INSTALL_HEADERS]
    QMAKE_CXXFLAGS += -isystem $$[QT_INSTALL_HEADERS]/QtCore
    QMAKE_CXXFLAGS += -isystem $$[QT_INSTALL_HEADERS]/QtGui

    QMAKE_CXXFLAGS += -isystem $$PWD/../../third-party/openvr/headers
    QMAKE_CXXFLAGS += -isystem $$PWD/../../third-party/easylogging++
}

# easylogging++ used to be a header only lib. Now requires easylogging++.cc
SOURCES += third-party/easylogging++/easylogging++.cc
