cmake_minimum_required(VERSION 3.27)
project(AdvancedSettings VERSION 5.7.4 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui Multimedia Qml Quick WebSockets Widgets QuickControls2 QuickDialogs2 Svg)
find_package(Qt${QT_VERSION_MAJOR} OPTIONAL_COMPONENTS DBus)

qt_standard_project_setup()

if(QT_KNOWN_POLICY_QTP0001)
    qt_policy(SET QTP0001 NEW)
endif()

# ability to just put .qrc files into the source list
set(CMAKE_AUTORCC ON)

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
    src/utils/paths.cpp
    src/utils/setup.cpp
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
    src/utils/paths.h
    src/utils/setup.h
    src/utils/update_rate.h
    src/utils/Matrix.h
    src/quaternion/quaternion.h
    )

set(QML_FILES
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
    src/res/qml/ChaperoneWarningsPage.qml
    src/res/qml/chaperone_page/ChaperonePage.qml
    src/res/qml/chaperone_page/change_orientation/ChangeOrientationGroupBox.qml
    src/res/qml/chaperone_page/chaperone_additional/ChaperoneAdditionalPage.qml
    src/res/qml/chaperone_page/chaperone_additional/chaperonemisc/ChaperoneMiscGroupBox.qml
    src/res/qml/chaperone_page/chaperone_additional/chaperonetype/ChaperoneTypeGroupBox.qml
    src/res/qml/chaperone_page/chaperone_additional/chaperoneboundscolor/ChaperoneBoundsColorGroupBox.qml
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
    src/res/qml/FixFloorPage.qml
    src/res/qml/common/MySlider.qml
    src/res/qml/common/MyResources.qml
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
    src/res/qml/common/MyPushButton2.qml
    src/res/qml/common/MyStackViewPage.qml
    src/res/qml/common/MyRadioButton.qml
    src/res/qml/common/FullWidthSliderBox.qml
    src/res/qml/common/MyDialogOkCancelPopup.qml
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

set(RESOURCES src/res/resources.qrc)

# make alias path relative to src/res/qml
foreach(qml_file ${QML_FILES})
    cmake_path(RELATIVE_PATH qml_file BASE_DIRECTORY src/res/qml OUTPUT_VARIABLE qml_alias)
    set_source_files_properties(${qml_file} PROPERTIES QT_RESOURCE_ALIAS ${qml_alias})
endforeach()

set_source_files_properties(src/res/qml/common/MyResources.qml PROPERTIES QT_QML_SINGLETON_TYPE TRUE)

qt_add_executable(AdvancedSettings WIN32 MACOSX_BUNDLE ${SRCS} ${RESOURCES} ${HEADERS})

target_include_directories(AdvancedSettings PUBLIC
    src)
target_include_directories(AdvancedSettings SYSTEM PUBLIC
    third-party/nlohmann
    third-party/openvr/headers)

# Avoid collision between executable name and QML module directory (QTBUG-98568).
if(NOT WIN32 AND NOT APPLE)
    set_property(TARGET AdvancedSettings PROPERTY OUTPUT_NAME AdvancedSettings_app)
endif()

target_compile_definitions(AdvancedSettings PRIVATE
    APPLICATION_VERSION=\"${PROJECT_VERSION}\"
)

find_library(OPENVR_LIBRARIES
    NAMES
      openvr_api
    PATHS
      ${CMAKE_CURRENT_SOURCE_DIR}/third-party/openvr/bin
      ${CMAKE_CURRENT_SOURCE_DIR}/third-party/openvr/lib
    PATH_SUFFIXES
      linux64
      win64
    NO_DEFAULT_PATH
    NO_CMAKE_FIND_ROOT_PATH)

target_link_libraries(AdvancedSettings PRIVATE
    Qt::Core
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
    ${OPENVR_LIBRARIES}
)

qt_add_qml_module(AdvancedSettings
    URI AdvancedSettings
    VERSION ${PROJECT_VERSION}
    QML_FILES ${QML_FILES}
)


if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU") # GCC
    target_compile_options(AdvancedSettings
        PUBLIC
        -Wall
        -Wpedantic
        -Wextra
        -pedantic-errors
        -Wcast-align
        -Wconversion
        -Wdouble-promotion
        -Wformat=2
        -Wno-sign-conversion
        -Wnon-virtual-dtor
        -Wnull-dereference
        -Wold-style-cast
        -Woverloaded-virtual
        -Wshadow
        -Wzero-as-null-pointer-constant
        -flto
    )

    if(warnings_as_errors)
        target_compile_options(AdvancedSettings
            -Werror
        )
    endif()
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(AdvancedSettings
	PUBLIC
        --system-header-prefix=qt
        --system-header-prefix=third-party
        -Wcast-align
        -Wconversion
        -Wdouble-promotion
        -Wextra
        -Wextra-semi
        -Wformat=2
        -Wmissing-variable-declarations
        -Wmost
        -Wno-sign-conversion
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Woverloaded-virtual
        -Wpedantic
        -Wshadow
        -Wunused
        -Wzero-as-null-pointer-constant
        -flto
    )
    if(warnings_as_errors)
        target_compile_options(AdvancedSettings
            -Werror
        )
    endif()
endif()

if(debugSymbolsAndLogs)
  target_compile_definitions(AdvancedSettings PRIVATE ENABLE_DEBUG_LOGGING)
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    cmake_path(GET QT_DIR PARENT_PATH QT_ROOT)
    cmake_path(GET QT_ROOT PARENT_PATH QT_ROOT)
    cmake_path(GET QT_ROOT PARENT_PATH QT_ROOT)

    cmake_path(NATIVE_PATH QT_ROOT NORMALIZE NATIVE_QT_ROOT)
    cmake_path(NATIVE_PATH QT_DIR NORMALIZE NATIVE_QT_DIR)
    cmake_path(NATIVE_PATH PROJECT_SOURCE_DIR NORMALIZE NATIVE_PROJECT_SOURCE_DIR)

    # Needs to be executed: Set-ExecutionPolicy -ExecutionPolicy Unrestricted -Scope Process
    configure_file(build_scripts/win/env.ps1.in env.ps1)
    configure_file(build_scripts/win/env.sh.in env.sh @ONLY)

    # Adds default qt plugins path
    configure_file(build_scripts/win/qt.conf.in qt.conf ESCAPE_QUOTES)

    target_sources(AdvancedSettings PUBLIC
        src/keyboard_input/input_sender_win.cpp
        src/media_keys/media_keys_win.cpp
        src/tabcontrollers/audiomanager/AudioManagerWindows.cpp
    )

    target_link_libraries(AdvancedSettings PRIVATE
        ole32
        user32
    )
    target_include_directories(AdvancedSettings PUBLIC third-party/policyconfig)

    target_compile_definitions(AdvancedSettings PRIVATE
            _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
        )
    if(MSVC)
        target_compile_options(AdvancedSettings PUBLIC
            /Gw
            /W4
            /permissive-
            /w14254
            /w14263
            /w14265
            /w14287
            /w14296
            /w14311
            /w14545
            /w14546
            /w14547
            /w14549
            /w14555
            /w14640
            /w14826
            /w14905
            /w14906
            /w14928
            /wd4127
            /we4289
        )

        if(warnings_as_errors)
            target_compile_options(AdvancedSettings
                /WX
            )
        endif()
    elseif(win32-clang-msvc)
        target_compile_options(AdvancedSettings
            -Wcast-align
            -Wdouble-promotion
            -Wextra
            -Wformat=2
            -Wmissing-variable-declarations
            -Wmost
            -Wnon-virtual-dtor
            -Wold-style-cast
            -Woverloaded-virtual
            -Wshadow
            -Wunused
            -flto
            /Gw
            /W4
            /imsvc /imsvc /imsvc
            /w14254
            /w14263
            /w14265
            /w14287
            /w14296
            /w14311
            /w14545
            /w14546
            /w14547
            /w14549
            /w14555
            /w14640
            /w14826
            /w14905
            /w14906
            /w14928
            /wd4127
            /we4289
        )
    endif()

elseif(UNIX AND NOT MACOS)
    if(noX11)
        target_sources(AdvancedSettings PUBLIC
            src/keyboard_input/input_sender_dummy.cpp
        )
    else()
        target_sources(AdvancedSettings PUBLIC
            src/keyboard_input/input_sender_X11.cpp
        )

        target_link_libraries(AdvancedSettings PRIVATE
            Xtst
            X11
        )
    endif()

    if(noDBUS)
        target_sources(AdvancedSettings PUBLIC
            src/media_keys/media_keys_dummy.cpp
        )
    else()
        target_sources(AdvancedSettings PUBLIC
            src/media_keys/media_keys_dbus.cpp
        )

        target_link_libraries(AdvancedSettings PRIVATE
            Qt::DBus
        )
    endif()

    if(noPulse)
        target_sources(AdvancedSettings PUBLIC
            src/tabcontrollers/audiomanager/AudioManagerDummy.cpp
        )
    else()
        target_sources(AdvancedSettings PUBLIC
            src/tabcontrollers/audiomanager/AudioManagerPulse.cpp
        )

        target_link_libraries(AdvancedSettings PRIVATE
            pulse
        )
    endif()
elseif(MACOS)
    target_sources(AdvancedSettings PUBLIC
        src/keyboard_input/input_sender_dummy.cpp
        src/media_keys/media_keys_dummy.cpp
        src/tabcontrollers/audiomanager/AudioManagerDummy.cpp
    )
endif()

# XXX: This is a hack, bcs SKIP_LINTING in qt is not working, but this doesn't work on
# github actions because the files are not generated before this glob is used
# fix this when this issue is resolved: https://bugreports.qt.io/browse/QTBUG-127578
file(GLOB_RECURSE skip "${CMAKE_CURRENT_BINARY_DIR}/.rcc/qmlcache/**/*.cpp" "${CMAKE_CURRENT_BINARY_DIR}/.rcc/qmlcache/*.cpp")
message("${CMAKE_BINARY_DIR}: ${skip}")
set_property(SOURCE ${skip} PROPERTY SKIP_LINTING ON)

# Get all propreties that cmake supports
execute_process(COMMAND cmake --help-property-list OUTPUT_VARIABLE CMAKE_PROPERTY_LIST)

# Convert command output into a CMake list
STRING(REGEX REPLACE ";" "\\\\;" CMAKE_PROPERTY_LIST "${CMAKE_PROPERTY_LIST}")
STRING(REGEX REPLACE "\n" ";" CMAKE_PROPERTY_LIST "${CMAKE_PROPERTY_LIST}")
# Fix https://stackoverflow.com/questions/32197663/how-can-i-remove-the-the-location-property-may-not-be-read-from-target-error-i
list(FILTER CMAKE_PROPERTY_LIST EXCLUDE REGEX "^LOCATION$|^LOCATION_|_LOCATION$")
# For some reason, "TYPE" shows up twice - others might too?
list(REMOVE_DUPLICATES CMAKE_PROPERTY_LIST)

# build whitelist by filtering down from CMAKE_PROPERTY_LIST in case cmake is
# a different version, and one of our hardcoded whitelisted properties
# doesn't exist!
unset(CMAKE_WHITELISTED_PROPERTY_LIST)
foreach(prop ${CMAKE_PROPERTY_LIST})
    if(prop MATCHES "^(INTERFACE|[_a-z]|IMPORTED_LIBNAME_|MAP_IMPORTED_CONFIG_)|^(COMPATIBLE_INTERFACE_(BOOL|NUMBER_MAX|NUMBER_MIN|STRING)|EXPORT_NAME|IMPORTED(_GLOBAL|_CONFIGURATIONS|_LIBNAME)?|NAME|TYPE|NO_SYSTEM_FROM_IMPORTED)$")
        list(APPEND CMAKE_WHITELISTED_PROPERTY_LIST ${prop})
    endif()
endforeach(prop)

function(print_properties)
    message ("CMAKE_PROPERTY_LIST = ${CMAKE_PROPERTY_LIST}")
endfunction(print_properties)

function(print_whitelisted_properties)
    message ("CMAKE_WHITELISTED_PROPERTY_LIST = ${CMAKE_WHITELISTED_PROPERTY_LIST}")
endfunction(print_whitelisted_properties)

function(print_target_properties tgt)

    get_source_file_property(target_type ${tgt} TYPE)
    if(target_type STREQUAL "INTERFACE_LIBRARY")
        set(PROP_LIST ${CMAKE_WHITELISTED_PROPERTY_LIST})
    else()
        set(PROP_LIST ${CMAKE_PROPERTY_LIST})
    endif()

    foreach (prop ${PROP_LIST})
        string(REPLACE "<CONFIG>" "${CMAKE_BUILD_TYPE}" prop ${prop})
        # message ("Checking ${prop}")
	get_property(propval SOURCE ${tgt} PROPERTY ${prop} SET)
        if (propval)
            get_source_file_property(propval ${tgt} ${prop})
            message ("${tgt} ${prop} = ${propval}")
        endif()
    endforeach(prop)
endfunction(print_target_properties)
print_target_properties("${CMAKE_CURRENT_BINARY_DIR}/.rcc/qmlcache/AdvancedSettings_src/res/qml/utilities_page/media_keys/MediaButton_qml.cpp")
