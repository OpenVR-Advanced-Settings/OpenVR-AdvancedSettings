cmake_minimum_required(VERSION 3.20)
project(AdvancedSettings VERSION 5.7.4 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui Multimedia Qml Quick WebSockets Widgets)
find_package(Qt${QT_VERSION_MAJOR} OPTIONAL_COMPONENTS DBus)

qt_standard_project_setup()

qt_add_executable(AdvancedSettings WIN32 MACOSX_BUNDLE
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
qt_add_qml_module(AdvancedSettings
    URI AdvancedSettings
    VERSION ${PROJECT_VERSION}
)


target_include_directories(AdvancedSettings PUBLIC
    src
    third-party/nlohmann
    third-party/openvr/headers)

# Avoid collision between executable name and QML module directory (QTBUG-98568).
if(NOT WIN32 AND NOT APPLE)
    set_property(TARGET AdvancedSettings PROPERTY OUTPUT_NAME AdvancedSettings_app)
endif()

target_compile_definitions(AdvancedSettings PRIVATE
    APPLICATION_VERSION=\"${PROJECT_VERSION}\"
)

target_link_libraries(AdvancedSettings PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Multimedia
    Qt::Qml
    Qt::Quick
    Qt::WebSockets
    Qt::Widgets
    openvr_api
)


# Resources:
set_source_files_properties("src/res/img/common/backarrow.svg"
    PROPERTIES QT_RESOURCE_ALIAS "backarrow"
)
set_source_files_properties("src/res/img/common/check.svg"
    PROPERTIES QT_RESOURCE_ALIAS "box_checkmark"
)
set(resources_resource_files
    "src/res/img/common/backarrow.svg"
    "src/res/img/common/check.svg"
)

qt_add_resources(AdvancedSettings "resources"
    PREFIX
        "/common"
    BASE
        "src/res"
    FILES
        ${resources_resource_files}
)
set_source_files_properties("src/res/img/audio/microphone/mic_off.svg"
    PROPERTIES QT_RESOURCE_ALIAS "off"
)
set_source_files_properties("src/res/img/audio/microphone/mic_on.svg"
    PROPERTIES QT_RESOURCE_ALIAS "on"
)
set(resources1_resource_files
    "src/res/img/audio/microphone/mic_off.svg"
    "src/res/img/audio/microphone/mic_on.svg"
)

qt_add_resources(AdvancedSettings "resources1"
    PREFIX
        "/microphone"
    BASE
        "src/res"
    FILES
        ${resources1_resource_files}
)
set_source_files_properties("src/res/img/audio/speaker/speaker_off.svg"
    PROPERTIES QT_RESOURCE_ALIAS "off"
)
set_source_files_properties("src/res/img/audio/speaker/speaker_on.svg"
    PROPERTIES QT_RESOURCE_ALIAS "on"
)
set(resources2_resource_files
    "src/res/img/audio/speaker/speaker_off.svg"
    "src/res/img/audio/speaker/speaker_on.svg"
)

qt_add_resources(AdvancedSettings "resources2"
    PREFIX
        "/speaker"
    BASE
        "src/res"
    FILES
        ${resources2_resource_files}
)
set_source_files_properties("src/res/img/audio/media_keys/outline_play_pause_white_24dp.svg"
    PROPERTIES QT_RESOURCE_ALIAS "play_pause"
)
set_source_files_properties("src/res/img/audio/media_keys/outline_skip_next_white_24dp.svg"
    PROPERTIES QT_RESOURCE_ALIAS "next"
)
set_source_files_properties("src/res/img/audio/media_keys/outline_skip_previous_white_24dp.svg"
    PROPERTIES QT_RESOURCE_ALIAS "previous"
)
set_source_files_properties("src/res/img/audio/media_keys/outline_stop_white_24dp.svg"
    PROPERTIES QT_RESOURCE_ALIAS "stop"
)
set(resources3_resource_files
    "src/res/img/audio/media_keys/outline_play_pause_white_24dp.svg"
    "src/res/img/audio/media_keys/outline_skip_next_white_24dp.svg"
    "src/res/img/audio/media_keys/outline_skip_previous_white_24dp.svg"
    "src/res/img/audio/media_keys/outline_stop_white_24dp.svg"
)

qt_add_resources(AdvancedSettings "resources3"
    PREFIX
        "/media_keys"
    BASE
        "src/res"
    FILES
        ${resources3_resource_files}
)
set_source_files_properties("src/res/img/main_menu_icons/audio_tab_icon.svg"
    PROPERTIES QT_RESOURCE_ALIAS "audio_tab_icon"
)
set_source_files_properties("src/res/img/main_menu_icons/bindings_tab_icon.svg"
    PROPERTIES QT_RESOURCE_ALIAS "bindings_tab_icon"
)
set_source_files_properties("src/res/img/main_menu_icons/chaperone_tab_icon.svg"
    PROPERTIES QT_RESOURCE_ALIAS "chaperone_tab_icon"
)
set_source_files_properties("src/res/img/main_menu_icons/motion_tab_icon.svg"
    PROPERTIES QT_RESOURCE_ALIAS "motion_tab_icon"
)
set_source_files_properties("src/res/img/main_menu_icons/offsets_tab_icon.svg"
    PROPERTIES QT_RESOURCE_ALIAS "offsets_tab_icon"
)
set_source_files_properties("src/res/img/main_menu_icons/rotation_tab_icon.svg"
    PROPERTIES QT_RESOURCE_ALIAS "rotation_tab_icon"
)
set_source_files_properties("src/res/img/main_menu_icons/settings_tab_icon.svg"
    PROPERTIES QT_RESOURCE_ALIAS "settings_tab_icon"
)
set_source_files_properties("src/res/img/main_menu_icons/space_fix_tab_icon.svg"
    PROPERTIES QT_RESOURCE_ALIAS "space_fix_tab_icon"
)
set_source_files_properties("src/res/img/main_menu_icons/statistics_tab_icon.svg"
    PROPERTIES QT_RESOURCE_ALIAS "statistics_tab_icon"
)
set_source_files_properties("src/res/img/main_menu_icons/steamvr_tab_icon.svg"
    PROPERTIES QT_RESOURCE_ALIAS "steamvr_tab_icon"
)
set_source_files_properties("src/res/img/main_menu_icons/utilities_tab_icon.svg"
    PROPERTIES QT_RESOURCE_ALIAS "utilities_tab_icon"
)
set_source_files_properties("src/res/img/main_menu_icons/video_tab_icon.svg"
    PROPERTIES QT_RESOURCE_ALIAS "video_tab_icon"
)
set(resources4_resource_files
    "src/res/img/main_menu_icons/audio_tab_icon.svg"
    "src/res/img/main_menu_icons/bindings_tab_icon.svg"
    "src/res/img/main_menu_icons/chaperone_tab_icon.svg"
    "src/res/img/main_menu_icons/motion_tab_icon.svg"
    "src/res/img/main_menu_icons/offsets_tab_icon.svg"
    "src/res/img/main_menu_icons/rotation_tab_icon.svg"
    "src/res/img/main_menu_icons/settings_tab_icon.svg"
    "src/res/img/main_menu_icons/space_fix_tab_icon.svg"
    "src/res/img/main_menu_icons/statistics_tab_icon.svg"
    "src/res/img/main_menu_icons/steamvr_tab_icon.svg"
    "src/res/img/main_menu_icons/utilities_tab_icon.svg"
    "src/res/img/main_menu_icons/video_tab_icon.svg"
)

qt_add_resources(AdvancedSettings "resources4"
    PREFIX
        "/main_menu_icons"
    BASE
        "src/res"
    FILES
        ${resources4_resource_files}
)

if(WIN32)
    target_sources(AdvancedSettings PUBLIC
        src/keyboard_input/input_sender_win.cpp
        src/media_keys/media_keys_win.cpp
        src/tabcontrollers/audiomanager/AudioManagerWindows.cpp
    )

    target_link_libraries(AdvancedSettings PRIVATE
        # Remove: L"${CMAKE_CURRENT_SOURCE_DIR}/../..//third-party/openvr/lib/win64"
        ole32
        user32
    )
    if(win32-msvc)
        target_include_directories(AdvancedSettings PRIVATE
            third-party/policyconfig
        )

        target_compile_definitions(AdvancedSettings PRIVATE
            _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
        )

        target_compile_options(AdvancedSettings
            /GL
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
    endif()

    if(win32-clang-msvc)
        target_compile_definitions(AdvancedSettings PRIVATE
            _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
        )

        target_compile_options(AdvancedSettings
            ${CMAKE_CURRENT_SOURCE_DIR}/../../third-party/openvr/headers
            ${CMAKE_CURRENT_SOURCE_DIR}/../../third-party/policyconfig
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
endif()

if(UNIX)
    target_link_libraries(AdvancedSettings PRIVATE
        # Remove: L"${CMAKE_CURRENT_SOURCE_DIR}/../..//third-party/openvr/lib/linux64"
    )
endif()


if(GCC)
    target_compile_options(AdvancedSettings
        -Wcast-align
        -Wconversion
        -Wdouble-promotion
        -Wduplicated-branches
        -Wduplicated-cond
        -Wextra
        -Wformat=2
        -Wlogical-op
        -Wno-sign-conversion
        -Wnon-virtual-dtor
        -Wnull-dereference
        -Wold-style-cast
        -Woverloaded-virtual
        -Wpedantic
        -Wrestrict
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

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(AdvancedSettings
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
    target_compile_definitions(AdvancedSettings PRIVATE
        ENABLE_DEBUG_LOGGING
    )
endif()

if(UNIX AND NOT MACOS)
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
endif()

if(MACOS)
    target_sources(AdvancedSettings PUBLIC
        src/keyboard_input/input_sender_dummy.cpp
        src/media_keys/media_keys_dummy.cpp
        src/tabcontrollers/audiomanager/AudioManagerDummy.cpp
    )
endif()

# done by Qt
#if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
#    target_compile_options(AdvancedSettings
#        -isystem ${Qt${QT_VERSION_MAJOR}Core_PRIVATE_INCLUDE_DIRS}
#        #-isystem $$[QT_INSTALL_HEADERS]/QtCore
#        #-isystem $$[QT_INSTALL_HEADERS]/QtGui
#        -isystem ${CMAKE_CURRENT_SOURCE_DIR}/build_scripts/qt/../../third-party/openvr/headers
#    )
#endif()

install(TARGETS AdvancedSettings
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Consider using qt_generate_deploy_app_script() for app deployment if
# the project can use Qt 6.3. In that case rerun qmake2cmake with
# --min-qt-version=6.3.
