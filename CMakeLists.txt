cmake_minimum_required(VERSION 4.1)
#app version will be auto-deployed to app
if(NOT DEFINED OVRAS_VERSION)
    set(OVRAS_VERSION 6.0.0)
endif()

project(AdvancedSettings VERSION ${OVRAS_VERSION} LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_VS_INCLUDE_INSTALL_TO_DEFAULT_BUILD 1)



#find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core)
#find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui Multimedia Qml Quick WebSockets Widgets QuickControls2 QuickDialogs2 Svg)
#find_package(Qt${QT_VERSION_MAJOR} OPTIONAL_COMPONENTS DBus)
find_package(Qt6 REQUIRED COMPONENTS Gui Multimedia Qml Quick WebSockets Widgets QuickControls2 QuickDialogs2 Svg GuiPrivate)
find_package(Vulkan REQUIRED)

qt_standard_project_setup(REQUIRES 6.8)

#qt_standard_project_setup should cover these
#include(GNUInstallDirs)
#set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTOUIC ON)

qt6_policy(SET QTP0001 NEW)
qt6_policy(SET QTP0004 NEW)


list(APPEND QML_IMPORT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/res/qml")
message(STATUS "QML_IMPORT_PATH ${QML_IMPORT_PATH}")


# ability to just put .qrc files into the source list
#set(CMAKE_AUTORCC ON)


set(SRCS
    src/alarm_clock/vr_alarm.cpp
    src/keyboard_input/input_parser.cpp
    src/keyboard_input/keyboard_input.cpp
    src/main.cpp
    src/openvr/ivrinput.cpp
    src/openvr/lh_console_util.cpp
    src/openvr/openvr_init.cpp
    src/openvr/ovr_application_wrapper.cpp
    src/openvr/ovr_overlay_wrapper.cpp
    src/openvr/ovr_settings_wrapper.cpp
    src/openvr/ovr_system_wrapper.cpp
    src/overlaycontroller.cpp
    src/settings/settings.cpp
    src/settings/settings_object.cpp
    src/tabcontrollers/AudioTabController.cpp
    src/tabcontrollers/ChaperoneTabController.cpp
    src/tabcontrollers/FixFloorTabController.cpp
    src/tabcontrollers/MoveCenterTabController.cpp
    src/tabcontrollers/RotationTabController.cpp
    src/tabcontrollers/SettingsTabController.cpp
    src/tabcontrollers/StatisticsTabController.cpp
    src/tabcontrollers/SteamVRTabController.cpp
    src/tabcontrollers/UtilitiesTabController.cpp
    src/tabcontrollers/VideoTabController.cpp
    src/utils/ChaperoneUtils.cpp
    src/utils/FrameRateUtils.cpp
    src/utils/ImageUtils.cpp
    src/utils/paths.cpp
    src/utils/setup.cpp
    src/utils/sound_player.cpp
    src/utils/update_rate.cpp
)

set(HEADERS
    src/alarm_clock/vr_alarm.h
    src/keyboard_input/input_parser.h
    src/keyboard_input/keyboard_input.h
    #TODO, check if we need this
    #src/keyboard_input/keyboard_sender.h
    src/openvr/ivrinput.h
    src/openvr/ivrinput_action.h
    src/openvr/ivrinput_input_source.h
    src/openvr/ivrinput_manifest.h
    src/openvr/lh_console_util.h
    src/openvr/openvr_init.h
    src/openvr/ovr_application_wrapper.h
    src/openvr/ovr_overlay_wrapper.h
    src/openvr/ovr_settings_wrapper.h
    src/openvr/ovr_system_wrapper.h
    src/overlaycontroller.h
    src/settings/settings.h
    src/settings/settings_object.h
    src/settings/internal/bool_setting_value.h
    src/settings/internal/double_setting_value.h
    src/settings/internal/setting_value.h
    src/settings/internal/settings_controller.h
    src/settings/internal/settings_object_data.h
    src/settings/internal/specific_setting_value.h
    src/tabcontrollers/AudioTabController.h
    src/tabcontrollers/ChaperoneTabController.h
    src/tabcontrollers/FixFloorTabController.h
    src/tabcontrollers/MoveCenterTabController.h
    src/tabcontrollers/RotationTabController.h
    src/tabcontrollers/SettingsTabController.h
    src/tabcontrollers/StatisticsTabController.h
    src/tabcontrollers/SteamVRTabController.h
    src/tabcontrollers/UtilitiesTabController.h
    src/tabcontrollers/VideoTabController.h
    src/utils/ChaperoneUtils.h
    src/utils/FrameRateUtils.h
    src/utils/ImageUtils.h
    src/utils/paths.h
    src/utils/setup.h
    src/utils/sound_player.h
    src/utils/update_rate.h
    src/utils/Matrix.h
    src/quaternion/quaternion.h
    )

set(QML_FILERC
    src/res/qml/utilities_page/keyboard_utils/KeyboardGroupBox.qml
    src/res/qml/utilities_page/media_keys/MediaControllerKeys.qml
    src/res/qml/utilities_page/media_keys/MediaButton.qml
    src/res/qml/utilities_page/misc/MiscBox.qml
    src/res/qml/utilities_page/alarm_clock/AlarmGroupBox.qml
    src/res/qml/utilities_page/UtilitiesPage.qml
    src/res/qml/motion_page/space_drag/SpaceDragGroupBox.qml
    src/res/qml/motion_page/height_toggle/HeightToggleGroupBox.qml
    src/res/qml/motion_page/gravity/GravityGroupBox.qml
    src/res/qml/motion_page/MotionPage.qml
    src/res/qml/PlayspacePage.qml
    src/res/qml/SettingsPage.qml
    src/res/qml/chaperone_page/ChaperonePage.qml
    src/res/qml/chaperone_page/change_orientation/ChangeOrientationGroupBox.qml
    src/res/qml/chaperone_page/proximity_warnings/ProximityWarningsGroupBox.qml
    src/res/qml/chaperone_page/chaperonemisc/ChaperoneMiscGroupBox.qml
    src/res/qml/chaperone_page/chaperoneboundscolor/ChaperoneBoundsColorGroupBox.qml
    src/res/qml/RootPage.qml
    src/res/qml/steamvr_page/steamvrmisc/SteamVRMiscGroupBox.qml
    src/res/qml/steamvr_page/SteamVRPage.qml
    src/res/qml/steamvr_page/camera/CameraGroupBox.qml
    src/res/qml/steamvr_page/steamvr_additional/SteamVRTXRXPage.qml
    src/res/qml/steamvr_page/steamvrbind/SteamVRBindGroupBox.qml
    src/res/qml/audio_page/device_selector/MicDeviceSelector.qml
    src/res/qml/audio_page/device_selector/AudioDeviceSelector.qml
    src/res/qml/audio_page/device_selector/MirrorDeviceSelector.qml
    src/res/qml/audio_page/device_selector/MirrorVolumeSlider.qml
    src/res/qml/audio_page/device_selector/MicVolumeSlider.qml
    src/res/qml/audio_page/dialog_boxes/AudioDeleteProfileDialog.qml
    src/res/qml/audio_page/dialog_boxes/AudioNewProfileDialog.qml
    src/res/qml/audio_page/dialog_boxes/AudioMessageDialog.qml
    src/res/qml/audio_page/proximity/ProximityToggle.qml
    src/res/qml/audio_page/profiles/ProfileButtons.qml
    src/res/qml/audio_page/AudioPage.qml
    src/res/qml/audio_page/push_to_talk/PttButtons.qml
    src/res/qml/utilities_page/space_fix/SpaceFixGroupBox.qml
    src/res/qml/common/MySlider.qml
    src/res/qml/common/MyPushButton.qml
    src/res/qml/common/HourComboBox.qml
    src/res/qml/common/MyText.qml
    src/res/qml/common/MyToggleButton.qml
    src/res/qml/common/MinuteSecondComboBox.qml
    src/res/qml/common/MyComboBox.qml
    src/res/qml/common/MyTextField.qml
    src/res/qml/common/TimeAssembly.qml
    src/res/qml/common/MyDialogOkPopup.qml
    src/res/qml/common/mainwidget.qml
    src/res/qml/common/LineSeparator.qml
    src/res/qml/common/VerticalSeparator.qml
    src/res/qml/common/MyPushButton2.qml
    src/res/qml/common/MyStackViewPage.qml
    src/res/qml/common/MyRadioButton.qml
    src/res/qml/common/FullWidthSliderBox.qml
    src/res/qml/common/MyDialogOkCancelPopup.qml
    src/res/qml/common/SettingsGroupBox.qml
    src/res/qml/rotation_page/view_ratchet/ViewRatchetGroupBox.qml
    src/res/qml/rotation_page/redirected/RedirectedGroupBox.qml
    src/res/qml/rotation_page/redirected/AutoTurnGroupBox.qml
    src/res/qml/rotation_page/snap_turn/SnapTurnGroupBox.qml
    src/res/qml/rotation_page/space_turn/SpaceTurnGroupBox.qml
    src/res/qml/rotation_page/RotationPage.qml
    src/res/qml/video_page/brightness/BrightnessGroupBox.qml
    src/res/qml/video_page/profiles/VideoProfileGroupBox.qml
    src/res/qml/video_page/overlaycolorbright/OverlayMethodGroupBox.qml
    src/res/qml/video_page/miscvid/VideoMiscGroupBox.qml
    src/res/qml/video_page/supersampling/SuperSamplingGroupBox.qml
    src/res/qml/video_page/color/ColorGroupBox.qml
    src/res/qml/video_page/VideoPage.qml
    src/res/qml/video_page/dialogboxes/VideoNewProfileDialog.qml
    src/res/qml/video_page/dialogboxes/VideoMessageDialog.qml
    src/res/qml/video_page/dialogboxes/VideoDeleteProfileDialog.qml
    src/res/qml/StatisticsPage.qml
)

set(ResourcesRC
    src/res/img/alarm/alarm_activated.png
    src/res/img/audio/media_keys/outline_play_pause_white_24dp.svg
    src/res/img/audio/media_keys/outline_skip_next_white_24dp.svg
    src/res/img/audio/media_keys/outline_skip_previous_white_24dp.svg
    src/res/img/audio/media_keys/outline_stop_white_24dp.svg
    src/res/img/audio/microphone/mic_off.svg
    src/res/img/audio/microphone/mic_on.svg
    src/res/img/audio/microphone/ptm_notification.png
    src/res/img/audio/microphone/ptt_notification.png
    src/res/img/audio/speaker/speaker_off.svg
    src/res/img/audio/speaker/speaker_on.svg
    src/res/img/battery/battery_0.png
    src/res/img/battery/battery_1.png
    src/res/img/battery/battery_2.png
    src/res/img/battery/battery_3.png
    src/res/img/battery/battery_4.png
    src/res/img/battery/battery_5.png
    src/res/img/chaperone/centermark.png
    src/res/img/chaperone/centermarkl1.png
    src/res/img/chaperone/centermarkl2.png
    src/res/img/chaperone/centermarkl3.png
    src/res/img/chaperone/centermarkr1.png
    src/res/img/chaperone/centermarkr2.png
    src/res/img/chaperone/centermarkr3.png
    src/res/img/common/backarrow.svg
    src/res/img/common/check.svg
    src/res/img/icons/thumbicon.png
    src/res/img/main_menu_icons/audio_tab_icon.svg
    src/res/img/main_menu_icons/bindings_tab_icon.svg
    src/res/img/main_menu_icons/chaperone_tab_icon.svg
    src/res/img/main_menu_icons/discord_tab_icon.svg
    src/res/img/main_menu_icons/home_tab_icon.svg
    src/res/img/main_menu_icons/language_icon.svg
    src/res/img/main_menu_icons/motion_tab_icon.svg
    src/res/img/main_menu_icons/offsets_tab_icon.svg
    src/res/img/main_menu_icons/rotation_tab_icon.svg
    src/res/img/main_menu_icons/settings_tab_icon.svg
    src/res/img/main_menu_icons/space_fix_tab_icon.svg
    src/res/img/main_menu_icons/statistics_tab_icon.svg
    src/res/img/main_menu_icons/steamvr_tab_icon.svg
    src/res/img/main_menu_icons/utilities_tab_icon.svg
    src/res/img/main_menu_icons/video_tab_icon.svg
    src/res/img/main_menu_icons/youtube_tab_icon.svg
    src/res/img/rotation/block.svg
    src/res/sounds/alarm01.wav
    src/res/sounds/click.wav
    src/res/sounds/focus.wav
)

# Strip the "src/res/" prefix so resources land at :/qt/qml/AdvancedSettings/<path>
foreach(res_file IN LISTS QML_FILERC ResourcesRC)
    string(REGEX REPLACE "^src/res/" "" res_alias "${res_file}")
    set_source_files_properties(${res_file} PROPERTIES QT_RESOURCE_ALIAS "${res_alias}")
endforeach()

#qt_add_executable(AdvancedSettings WIN32 MACOSX_BUNDLE ${SRCS} ${HEADERS})
qt_add_executable(AdvancedSettings
    ${SRCS}
    ${HEADERS}
)

if (WIN32)
    configure_file(cmake/AdvancedSettings.rc.in "${CMAKE_CURRENT_BINARY_DIR}/AdvancedSettings.rc" @ONLY)
    target_sources(AdvancedSettings PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/AdvancedSettings.rc")
endif ()

qt_add_qml_module(AdvancedSettings
        URI AdvancedSettings
        QML_FILES
            ${QML_FILERC}
        RESOURCES
            ${ResourcesRC}
        VERSION ${PROJECT_VERSION}
)

target_include_directories(AdvancedSettings PUBLIC
    third-party/nlohmann
    third-party/openvr/headers
    ${Vulkan_INCLUDE_DIR}
)

# Avoid collision between executable name and QML module directory (QTBUG-98568).
if(NOT WIN32 AND NOT APPLE)
    set_property(TARGET AdvancedSettings PROPERTY OUTPUT_NAME AdvancedSettings_app)
endif()

target_compile_definitions(AdvancedSettings PRIVATE
    APPLICATION_VERSION=\"${PROJECT_VERSION}\"
)

#find_library(OPENVR_LIBRARIES
#    NAMES
#      openvr_api
#    PATHS
#      ${CMAKE_CURRENT_SOURCE_DIR}/third-party/openvr/bin
#      ${CMAKE_CURRENT_SOURCE_DIR}/third-party/openvr/lib
#    PATH_SUFFIXES
#      linux64
#      win64
#    NO_DEFAULT_PATH
#    NO_CMAKE_FIND_ROOT_PATH)

add_library(openvr_api SHARED IMPORTED)
if (WIN32)
    set_target_properties(openvr_api PROPERTIES
            IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/third-party/openvr/bin/win64/openvr_api.dll"
            IMPORTED_IMPLIB "${CMAKE_SOURCE_DIR}/third-party/openvr/lib/win64/openvr_api.lib"
    )
else ()
    # NO_SONAME links with -lopenvr_api so the binary needs "libopenvr_api.so"
    # instead of an absolute path into the source tree
    set_target_properties(openvr_api PROPERTIES
            IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/third-party/openvr/lib/linux64/libopenvr_api.so"
            IMPORTED_NO_SONAME TRUE
    )
endif ()

target_link_libraries(AdvancedSettings PRIVATE
    Qt::Gui
    # For QRhi
    Qt::GuiPrivate
    Qt::Multimedia
    Qt::Qml
    Qt::Quick
    Qt::WebSockets
    Qt::Widgets
    Qt::QuickControls2
    Qt::QuickDialogs2
    Qt::Svg
    openvr_api
    
)
#${OPENVR_LIBRARIES}


#qt_add_qml_module(AdvancedSettings
#    URI AdvancedSettings
#    VERSION ${PROJECT_VERSION}
#    QML_FILES ${QML_FILES}
#)

set(COPY_DEST_DIR "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>")
set_target_properties(AdvancedSettings PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${COPY_DEST_DIR}"
)

# readmeCopy
add_custom_command(TARGET AdvancedSettings POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/Readme.md" "${COPY_DEST_DIR}/Readme.md"
)

# licenseCopy
add_custom_command(TARGET AdvancedSettings POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/LICENSE" "${COPY_DEST_DIR}/LICENSE"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/third-party/openvr/LICENSE-VALVE" "${COPY_DEST_DIR}/LICENSE-VALVE"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/third-party/material-symbols/LICENSE-MATERIAL-SYMBOLS" "${COPY_DEST_DIR}/LICENSE-MATERIAL-SYMBOLS"
        #COMMAND ${CMAKE_COMMAND} -E copy_if_different
        #"${CMAKE_SOURCE_DIR}/third-party/magic_enum/LICENSE" "${COPY_DEST_DIR}/LICENSE-MAGIC-ENUM"
)

# packageFoldersCopy
add_custom_command(TARGET AdvancedSettings POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/src/package_files/default_action_manifests"
        "${COPY_DEST_DIR}/default_action_manifests"
)

# packageFilesCopy
add_custom_command(TARGET AdvancedSettings POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/src/package_files/action_manifest.json" "${COPY_DEST_DIR}/action_manifest.json"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/src/package_files/manifest.vrmanifest" "${COPY_DEST_DIR}/manifest.vrmanifest"
)
if (WIN32)
    add_custom_command(TARGET AdvancedSettings POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/src/package_files/restartvrserver.bat" "${COPY_DEST_DIR}/restartvrserver.bat"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/src/package_files/startdesktopmode.bat" "${COPY_DEST_DIR}/startdesktopmode.bat"
    )
endif ()

# openvrApiCopy
add_custom_command(TARGET AdvancedSettings POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:openvr_api>" "${COPY_DEST_DIR}/$<TARGET_FILE_NAME:openvr_api>"
)

# opensslCopy
if (WIN32)
    add_custom_command(TARGET AdvancedSettings POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/third-party/openssl/libcrypto-1_1-x64.dll" "${COPY_DEST_DIR}/libcrypto-1_1-x64.dll"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/third-party/openssl/libssl-1_1-x64.dll" "${COPY_DEST_DIR}/libssl-1_1-x64.dll"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/third-party/openssl/LICENSE-OPENSSL" "${COPY_DEST_DIR}/LICENSE-OPENSSL"
    )
endif ()

#Windows step actually puts all files in correct location
if (WIN32)
    set(WINDEPLOYQT "${Qt6_DIR}/../../../bin/windeployqt.exe")
    set(QT_BIN_DIR "${Qt6_DIR}/../../../bin")
    add_custom_command(TARGET AdvancedSettings POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E env "PATH=${QT_BIN_DIR};$ENV{PATH}"
            "${WINDEPLOYQT}"
            --qmldir "${CMAKE_SOURCE_DIR}/src/res/qml"
            "$<IF:$<CONFIG:Debug>,--debug,--release>"
            "${COPY_DEST_DIR}/AdvancedSettings.exe"
    )
endif ()

# Linux equivalent of windeployqt: run the install step (which includes the
# Qt deploy script below) into a self-contained folder with Qt libs, plugins
# and QML modules, so the result runs without Qt installed on the machine
if (UNIX AND NOT APPLE)
    set(LINUX_DEPLOY_DIR "${CMAKE_CURRENT_BINARY_DIR}/deploy/$<CONFIG>")
    # Start from an empty folder, install never removes stale files
    add_custom_command(TARGET AdvancedSettings POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E rm -rf "${LINUX_DEPLOY_DIR}"
            COMMAND ${CMAKE_COMMAND} --install "${CMAKE_CURRENT_BINARY_DIR}"
            --config $<CONFIG>
            --prefix "${LINUX_DEPLOY_DIR}"
            COMMENT "Deploying Qt runtime to ${LINUX_DEPLOY_DIR}"
    )
endif ()

target_include_directories(AdvancedSettings PRIVATE
        third-party/openvr/headers
        third-party/policyconfig
)

if (WIN32)
    target_sources(AdvancedSettings PRIVATE
            src/keyboard_input/input_sender_win.cpp
            src/media_keys/media_keys_win.cpp
            src/tabcontrollers/audiomanager/AudioManagerWindows.cpp
            src/tabcontrollers/audiomanager/AudioManagerWindows.h
    )

    target_link_libraries(AdvancedSettings PRIVATE
            ole32
            user32
    )

    if (MSVC)
        target_compile_definitions(AdvancedSettings PRIVATE
                _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
        )
        target_compile_options(AdvancedSettings PRIVATE
                /GL /Gw /W4 /permissive- /w14254 /w14263 /w14265 /w14287 /w14296 /w14311
                /w14545 /w14546 /w14547 /w14549 /w14555 /w14640 /w14826 /w14905 /w14906
                /w14928 /wd4127 /we4289 /Zc:__cplusplus
        )
        if (warnings_as_errors)
            target_compile_options(AdvancedSettings PRIVATE /WX)
        endif ()
    endif ()
elseif (UNIX)
    if (NOT APPLE AND NOT noX11)
        target_sources(AdvancedSettings PRIVATE
                src/keyboard_input/input_sender_X11.cpp
        )
        find_package(X11 REQUIRED)
        target_link_libraries(AdvancedSettings PRIVATE ${X11_LIBRARIES} X11::Xtst)
    elseif (NOT APPLE AND noX11)
        target_sources(AdvancedSettings PRIVATE
                src/keyboard_input/input_sender_dummy.cpp
        )
    endif ()

    if (NOT APPLE AND NOT noDBUS)
        target_sources(AdvancedSettings PRIVATE
                src/media_keys/media_keys_dbus.cpp
        )
        find_package(Qt6 OPTIONAL_COMPONENTS DBus)
        target_link_libraries(AdvancedSettings PRIVATE Qt6::DBus)
    elseif (NOT APPLE AND noDBUS)
        target_sources(AdvancedSettings PRIVATE
                src/media_keys/media_keys_dummy.cpp
        )
    endif ()

    if (NOT APPLE AND NOT noPulse)
        target_sources(AdvancedSettings PRIVATE
                src/tabcontrollers/audiomanager/AudioManagerPulse.cpp
                src/tabcontrollers/audiomanager/AudioManagerPulse.h
                src/tabcontrollers/audiomanager/AudioManagerPulse_internal.h
        )
        target_link_libraries(AdvancedSettings PRIVATE pulse)
    elseif (NOT APPLE AND noPulse)
        target_sources(AdvancedSettings PRIVATE
                src/tabcontrollers/audiomanager/AudioManagerDummy.cpp
                src/tabcontrollers/audiomanager/AudioManagerDummy.h
        )
    endif ()

    if (APPLE)
        target_sources(AdvancedSettings PRIVATE
                src/keyboard_input/input_sender_dummy.cpp
                src/media_keys/media_keys_dummy.cpp
                src/tabcontrollers/audiomanager/AudioManagerDummy.cpp
                src/tabcontrollers/audiomanager/AudioManagerDummy.h
        )
    endif ()

    if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(AdvancedSettings PRIVATE
                -Wcast-align
                -Wconversion
                -Wdouble-promotion
                -Wextra
                -Wformat=2
                -Wno-sign-conversion
                -Wnon-virtual-dtor
                -Wnull-dereference
                -Wold-style-cast
                -Woverloaded-virtual
                -Wpedantic
                -Wshadow
                -Wunused
                -Wzero-as-null-pointer-constant
                #-flto
        )
        # GCC-only warnings, clang rejects these as unknown
        if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            target_compile_options(AdvancedSettings PRIVATE
                    -Wduplicated-branches
                    -Wduplicated-cond
                    -Wlogical-op
                    -Wrestrict
            )
        endif ()
        if (warnings_as_errors)
            target_compile_options(AdvancedSettings PRIVATE -Werror)
        endif ()
    endif ()
endif ()

if (debugSymbolsAndLogs)
    target_compile_definitions(AdvancedSettings PRIVATE ENABLE_DEBUG_LOGGING)
endif ()

install(TARGETS AdvancedSettings
        BUNDLE DESTINATION .
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Windows loads DLLs from next to the exe, Linux finds the .so through the
# $ORIGIN/../lib RPATH set by qt_standard_project_setup
install(IMPORTED_RUNTIME_ARTIFACTS openvr_api
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# The app looks these up in its binary directory at runtime
install(FILES
        "${CMAKE_SOURCE_DIR}/Readme.md"
        "${CMAKE_SOURCE_DIR}/LICENSE"
        "${CMAKE_SOURCE_DIR}/third-party/openvr/LICENSE-VALVE"
        "${CMAKE_SOURCE_DIR}/third-party/material-symbols/LICENSE-MATERIAL-SYMBOLS"
        "${CMAKE_SOURCE_DIR}/src/package_files/action_manifest.json"
        "${CMAKE_SOURCE_DIR}/src/package_files/manifest.vrmanifest"
        DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(DIRECTORY "${CMAKE_SOURCE_DIR}/src/package_files/default_action_manifests"
        DESTINATION ${CMAKE_INSTALL_BINDIR}
)
if (WIN32)
    install(FILES
            "${CMAKE_SOURCE_DIR}/src/package_files/restartvrserver.bat"
            "${CMAKE_SOURCE_DIR}/src/package_files/startdesktopmode.bat"
            DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
endif ()

# Deploys Qt libraries, plugins and QML modules. On Linux, system libraries
# (glibc, libGL, libvulkan, X11/xcb, pulse, dbus) are excluded by default.
# qgtk3 is excluded because it needs GTK3, which the Steam Linux Runtime
# (steamrt4) does not ship; Qt falls back to its built-in theme without it.
qt_generate_deploy_qml_app_script(
    TARGET AdvancedSettings
    OUTPUT_SCRIPT deploy_script
    MACOS_BUNDLE_POST_BUILD
    EXCLUDE_PLUGINS qgtk3
)
install(SCRIPT ${deploy_script})
message(STATUS "Deploy Script ${deploy_script}")

if (WIN32)

# message(STATUS "Path to qml: ${CMAKE_SOURCE_DIR}/src/res/qml")
#This should be re-dundant but for some reason Visual Studio really wants this
    #set(WINDEPLOYQT_BUILD_TARGET "$<$<CONFIG:Debug>:--debug>$<$<NOT:$<CONFIG:Debug>>:--release>")
    #add_custom_command(TARGET AdvancedSettings POST_BUILD
    #        COMMAND "${Qt6_DIR}/../../../bin/windeployqt6.exe"
    #        --dir "${COPY_DEST_DIR}/qtdata"
    #        --libdir "${COPY_DEST_DIR}"
    #        --plugindir "${COPY_DEST_DIR}/qtdata/plugins"
    #        --no-system-d3d-compiler
    #        --no-opengl-sw
    #        ${WINDEPLOYQT_BUILD_TARGET}
    #        --qmldir "${CMAKE_SOURCE_DIR}/src/res/qml"
    #        "${COPY_DEST_DIR}/AdvancedSettings.exe"
    #)
endif ()
