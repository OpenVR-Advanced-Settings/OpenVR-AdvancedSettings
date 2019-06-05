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
    src/tabcontrollers/VideoTabController.cpp \
    src/utils/ChaperoneUtils.cpp \
    src/openvr/openvr_init.cpp \
    src/openvr/ivrinput.cpp \
    src/utils/setup.cpp \
    src/utils/paths.cpp \
    src/openvr/overlay_utils.cpp \
    src/keyboard_input/keyboard_input.cpp \
    src/keyboard_input/input_parser.cpp

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
    src/tabcontrollers/VideoTabController.h \
    src/tabcontrollers/AudioManager.h \
    src/tabcontrollers/PttController.h \
    src/keyboard_input/keyboard_input.h \
    src/media_keys/media_keys.h \
    src/utils/Matrix.h \
    src/utils/ChaperoneUtils.h \
    src/quaternion/quaternion.h \
    src/tabcontrollers/audiomanager/AudioManagerDummy.h \
    src/openvr/openvr_init.h \
    src/openvr/ivrinput_action.h \
    src/openvr/ivrinput_manifest.h \
    src/openvr/ivrinput_action_set.h \
    src/openvr/ivrinput_input_source.h \
    src/openvr/ivrinput.h \
    src/utils/setup.h \
    src/utils/paths.h \
    src/openvr/overlay_utils.h \
    src/keyboard_input/input_parser.h \
    src/keyboard_input/input_sender.h

win32 {
    SOURCES += src/tabcontrollers/audiomanager/AudioManagerWindows.cpp \
        src/keyboard_input/input_sender_win.cpp \
        src/media_keys/media_keys_win.cpp
    HEADERS += src/tabcontrollers/audiomanager/AudioManagerWindows.h
}

unix:!macx {
    !noX11 {
        SOURCES += src/keyboard_input/input_sender_X11.cpp
        CONFIG += x11
        LIBS += -lXtst
    }
    else {
        SOURCES += src/keyboard_input/input_sender_dummy.cpp
    }

    !noDBUS {
        SOURCES += src/media_keys/media_keys_dbus.cpp
        QT += dbus
    }
    else {
        SOURCES += src/media_keys/media_keys_dummy.cpp
    }

    SOURCES += src/tabcontrollers/audiomanager/AudioManagerDummy.cpp
    HEADERS += src/tabcontrollers/audiomanager/AudioManagerDummy.h
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
