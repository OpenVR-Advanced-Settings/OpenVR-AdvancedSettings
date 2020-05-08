project_dir = $$PWD/../../

win32:LIBS += -L"$$project_dir/third-party/openvr/lib/win64" -luser32 -lole32
unix:LIBS += -L"$$project_dir/third-party/openvr/lib/linux64"
LIBS += -lopenvr_api

RESOURCES += src/res/resources.qrc

DISTFILES += \
    src/res/sounds/alarm01.wav \
	src/res/sounds/click.wav \
	src/res/sounds/focus.wav \
    src/res/img/audio/microphone/ptt_notification.png \
    src/res/img/audio/microphone/ptt_notification.svg \
    src/res/img/battery/* \
    src/res/img/icons/* \
	src/res/img/video/* \
	src/res/img/chaperone/* \
    src/res/qml/qmldir \
    src/res/qml/audio_page/* \
    src/res/qml/ChaperonePage.qml \
    src/res/qml/ChaperoneWarningsPage.qml \
    src/res/qml/chaperone_page/* \
    src/res/qml/chaperone_page/change_orientation/* \
	src/res/qml/chaperone_page/chaperone_additional/* \
	src/res/qml/chaperone_page/chaperone_additional/chaperoneboundscolor/* \
	src/res/qml/chaperone_page/chaperone_additional/chaperonetype/* \
	src/res/qml/chaperone_page/chaperone_additional/chaperonemisc/* \
    src/res/qml/FixFloorPage.qml \
    src/res/qml/PlayspacePage.qml \
    src/res/qml/motion_page/* \
    src/res/qml/motion_page/gravity/* \
    src/res/qml/motion_page/height_toggle/* \
    src/res/qml/motion_page/space_drag/* \
    src/res/qml/RootPage.qml \
    src/res/qml/SettingsPage.qml \
    src/res/qml/StatisticsPage.qml \
    src/res/qml/steamvr_page/SteamVRPage.qml \
    src/res/qml/steamvr_page/camera/* \
    src/res/qml/steamvr_page/steamvrmisc/* \
    src/res/qml/utilities_page/* \
    src/res/qml/utilities_page/media_keys/* \
    src/res/qml/utilities_page/keyboard_utils/* \
    src/res/qml/utilities_page/alarm_clock/* \
    src/res/qml/audio_page/dialog_boxes/* \
    src/res/qml/audio_page/device_selector/* \
    src/res/qml/audio_page/proximity/* \
    src/res/qml/audio_page/push_to_talk/* \
    src/res/qml/audio_page/profiles/* \
    src/res/qml/video_page/* \
    src/res/qml/video_page/brightness/* \
    src/res/qml/video_page/color/* \
    src/res/qml/video_page/supersampling/* \
    src/res/qml/video_page/miscvid/* \
    src/res/qml/video_page/profiles/* \
    src/res/qml/video_page/dialogboxes/* \
    src/res/qml/video_page/overlaycolorbright/* \
	src/res/qml/rotation_page/* \
	src/res/qml/rotation_page/redirected/* \
	src/res/qml/rotation_page/snap_turn/* \
	src/res/qml/rotation_page/space_turn/* \
    src/res/qml/common/*

win32:RC_ICONS += src/res/img/icons/advicon256px.ico
