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
		src/tabcontrollers/audiomanager/AudioManagerWindows.cpp \
		src/tabcontrollers/PttController.cpp \
		src/tabcontrollers/AccessibilityTabController.cpp \
		src/utils/ChaperoneUtils.cpp \
    release/moc_AccessibilityTabController.cpp \
    release/moc_AudioTabController.cpp \
    release/moc_ChaperoneTabController.cpp \
    release/moc_FixFloorTabController.cpp \
    release/moc_MoveCenterTabController.cpp \
    release/moc_overlaycontroller.cpp \
    release/moc_PttController.cpp \
    release/moc_ReviveTabController.cpp \
    release/moc_SettingsTabController.cpp \
    release/moc_StatisticsTabController.cpp \
    release/moc_SteamVRTabController.cpp \
    release/moc_UtilitiesTabController.cpp \
    src/tabcontrollers/audiomanager/AudioManagerWindows.cpp \
    src/tabcontrollers/AccessibilityTabController.cpp \
    src/tabcontrollers/AudioTabController.cpp \
    src/tabcontrollers/ChaperoneTabController.cpp \
    src/tabcontrollers/FixFloorTabController.cpp \
    src/tabcontrollers/MoveCenterTabController.cpp \
    src/tabcontrollers/PttController.cpp \
    src/tabcontrollers/ReviveTabController.cpp \
    src/tabcontrollers/SettingsTabController.cpp \
    src/tabcontrollers/StatisticsTabController.cpp \
    src/tabcontrollers/SteamVRTabController.cpp \
    src/tabcontrollers/UtilitiesTabController.cpp \
    src/utils/ChaperoneUtils.cpp \
    src/main.cpp \
    src/overlaycontroller.cpp

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
		src/tabcontrollers/audiomanager/AudioManagerWindows.h \
		src/tabcontrollers/PttController.h \
		src/tabcontrollers/AccessibilityTabController.h \
		src/utils/Matrix.h \
		src/utils/ChaperoneUtils.h \
    release/moc_predefs.h \
    src/tabcontrollers/audiomanager/AudioManagerWindows.h \
    src/tabcontrollers/audiomanager/IPolicyConfig.h \
    src/tabcontrollers/AccessibilityTabController.h \
    src/tabcontrollers/AudioManager.h \
    src/tabcontrollers/AudioTabController.h \
    src/tabcontrollers/ChaperoneTabController.h \
    src/tabcontrollers/FixFloorTabController.h \
    src/tabcontrollers/MoveCenterTabController.h \
    src/tabcontrollers/PttController.h \
    src/tabcontrollers/ReviveTabController.h \
    src/tabcontrollers/SettingsTabController.h \
    src/tabcontrollers/StatisticsTabController.h \
    src/tabcontrollers/SteamVRTabController.h \
    src/tabcontrollers/UtilitiesTabController.h \
    src/utils/ChaperoneUtils.h \
    src/utils/Matrix.h \
    src/logging.h \
    src/overlaycontroller.h \
    third-party/easylogging++/easylogging++.h \
    third-party/openvr/bin/osx64/OpenVR.framework/Versions/A/Headers/openvr.h \
    third-party/openvr/bin/osx64/OpenVR.framework/Versions/A/Headers/openvr_capi.h \
    third-party/openvr/bin/osx64/OpenVR.framework/Versions/A/Headers/openvr_driver.h \
    third-party/openvr/headers/openvr.h \
    third-party/openvr/headers/openvr_capi.h \
    third-party/openvr/headers/openvr_driver.h

INCLUDEPATH += third-party/openvr/headers \
			third-party/easylogging++

win32:LIBS += -L"$$PWD/third-party/openvr/lib/win64"
unix:LIBS += -L"$$PWD/third-party/openvr/lib/linux64"
LIBS += -lopenvr_api -luser32 -lole32

DESTDIR = bin/win64/AdvancedSettings

DISTFILES += \
    src/res/qml/ptt_notification.png \
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
