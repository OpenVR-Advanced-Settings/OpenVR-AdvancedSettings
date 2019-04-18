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
    src/tabcontrollers/audiomanager/AudioManagerDummy.cpp \
    src/openvr/openvr_init.cpp \
    src/openvr/ivrinput.cpp \
    src/utils/setup.cpp \
    src/utils/paths.cpp \
    src/openvr/overlay_utils.cpp

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
    src/tabcontrollers/keyboardinput/KeyboardInput.h \
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
    src/openvr/overlay_utils.h

win32 {
    SOURCES += src/tabcontrollers/audiomanager/AudioManagerWindows.cpp \
        src/tabcontrollers/keyboardinput/KeyboardInputWindows.cpp
    HEADERS += src/tabcontrollers/audiomanager/AudioManagerWindows.h
}

unix:!macx {
    SOURCES += src/tabcontrollers/keyboardinput/KeyboardInputX11.cpp
    CONFIG += x11
    LIBS += -lXtst
}

macx {
    SOURCES += src/tabcontrollers/keyboardinput/KeyboardInputDummy.cpp
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
    QMAKE_CXXFLAGS += -isystem ../third-party/openvr/headers
    QMAKE_CXXFLAGS += -isystem ../third-party/easylogging++
}

# easylogging++ used to be a header only lib. Now requires easylogging++.cc
SOURCES += third-party/easylogging++/easylogging++.cc
