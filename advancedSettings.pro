#-------------------------------------------------
#
# Project created by QtCreator 2015-06-10T16:57:45
#
#-------------------------------------------------

QT       += core gui qml quick multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AdvancedSettings
TEMPLATE = app


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
                src/tabcontrollers/audiomanager/AudioManagerDummy.cpp

HEADERS  += src/overlaycontroller.h \
		src/logging.h \
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
		src/utils/Matrix.h \
		src/utils/ChaperoneUtils.h \
                src/tabcontrollers/audiomanager/AudioManagerDummy.h

win32 {
    SOURCES += src/tabcontrollers/audiomanager/AudioManagerWindows.h
    HEADERS += src/tabcontrollers/audiomanager/AudioManagerWindows.h
}

INCLUDEPATH += third-party/openvr/headers \
			third-party/easylogging++

win32:LIBS += -L"$$PWD/third-party/openvr/lib/win64"
unix:LIBS += -L"$$PWD/third-party/openvr/lib/linux64"
LIBS += -lopenvr_api -luser32 -lole32

DESTDIR = bin/win64/AdvancedSettings
