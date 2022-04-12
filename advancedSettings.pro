QT       += core gui qml quick multimedia widgets websockets
CONFIG   += c++1z file_copies optimize_full

DEFINES += ELPP_THREAD_SAFE ELPP_QT_LOGGING ELPP_NO_DEFAULT_LOG_FILE

VERSION = "$$cat($$PWD/build_scripts/compile_version_string.txt)"
DEFINES += APPLICATION_VERSION=\\\"$$VERSION\\\"
# Qmake can't handle non-integers in the version string,
# so we take out anything after the dash.
SPLIT_VERSION_STRING = $$split(VERSION, -)
NUMERICAL_VERSION_NUMBER = $$member(SPLIT_VERSION_STRING, 0, 0)
VERSION = $$NUMERICAL_VERSION_NUMBER

# Keep the literal version number in the description field for debugging.
QMAKE_TARGET_DESCRIPTION = "$$cat($$PWD/build_scripts/compile_version_string.txt)"

lessThan(QT_MAJOR_VERSION, 5): error("requires Qt 5.12 or higher")
lessThan(QT_MINOR_VERSION, 12): error("requires Qt 5.12 or higher")

TARGET = AdvancedSettings
TEMPLATE = app

win32:DESTDIR = bin/win64/AdvancedSettings
unix:DESTDIR = bin/linux/AdvancedSettings

include_dir = $$PWD/build_scripts/qt/

win32-msvc {
    include($$include_dir/compilers/msvc.pri)
}
win32-clang-msvc{
    include($$include_dir/compilers/clang-msvc.pri)
}
*g++* {
    include($$include_dir/compilers/gcc.pri)
}
#Look for anything clang that is not clang-msvc, since it does not
#allow all the same switches as regular clang.
*clang|*clang-g++|*clang-libc++ {
    include($$include_dir/compilers/clang.pri)
}

include($$include_dir/sources.pri)

include($$include_dir/resources.pri)

# Copy extra files
COPIES += resCopy readmeCopy licenseCopy packageFoldersCopy openvrApiCopy packageFilesCopy
COPY_DEST_DIR = $$OUT_PWD/$$DESTDIR

resCopy.files = $$files(src/res)
resCopy.path = $$COPY_DEST_DIR

readmeCopy.files = Readme.md
readmeCopy.path = $$COPY_DEST_DIR

licenseCopy.files = LICENSE \
                    third-party/openvr/LICENSE-VALVE  \
                    third-party/easylogging++/LICENSE-MIT
licenseCopy.path = $$COPY_DEST_DIR

packageFoldersCopy.files = src/package_files/default_action_manifests
packageFoldersCopy.path = $$COPY_DEST_DIR

packageFilesCopy.files = src/package_files/action_manifest.json src/package_files/manifest.vrmanifest
win32:packageFilesCopy.files += src/package_files/qt.conf src/package_files/restartvrserver.bat src/package_files/startdesktopmode.bat
packageFilesCopy.path = $$COPY_DEST_DIR

win32:openvrApiCopy.files = third-party/openvr/bin/win64/openvr_api.dll
unix:openvrApiCopy.files = third-party/openvr/lib/linux64/libopenvr_api.so
openvrApiCopy.path = $$COPY_DEST_DIR

# Deploy openssl when on win32
win32 {
    COPIES += opensslCopy
    opensslCopy.files = third-party/openssl/libcrypto-1_1-x64.dll
    opensslCopy.files += third-party/openssl/libssl-1_1-x64.dll
    opensslCopy.files += third-party/openssl/LICENSE-OPENSSL
    opensslCopy.path = $$COPY_DEST_DIR
}

# Deploy resources and DLLs to exe dir on Windows
win32 {
    WINDEPLOYQT_LOCATION = $$dirname(QMAKE_QMAKE)/windeployqt.exe

    CONFIG( debug, debug|release ) {
        WINDEPLOYQT_BUILD_TARGET += "--debug"
    } else {
        WINDEPLOYQT_BUILD_TARGET += "--release"
    }

    WINDEPLOYQT_OPTIONS = --dir $$COPY_DEST_DIR/qtdata \
                          --libdir $$COPY_DEST_DIR \
                          --plugindir $$COPY_DEST_DIR/qtdata/plugins \
                          --no-system-d3d-compiler \
                          --no-opengl-sw \
                          $$WINDEPLOYQT_BUILD_TARGET \
                          --qmldir $$PWD/src/res/qml \
                          $$COPY_DEST_DIR/AdvancedSettings.exe
    WINDEPLOYQT_FULL_LINE = "$$WINDEPLOYQT_LOCATION $$WINDEPLOYQT_OPTIONS"

    # Force windeployqt to run in cmd, because powershell has different syntax
    # for running executables.
    QMAKE_POST_LINK = cmd /c $$WINDEPLOYQT_FULL_LINE
}

# Add make install support
unix {
    isEmpty(PREFIX){
        PREFIX = /opt/OpenVR-AdvancedSettings
    }

    message(PREFIX value is: \'$$PREFIX\'. `make install` will install to this location.)

    application.path = $$PREFIX
    application.files = $$COPY_DEST_DIR

    INSTALLS += application
}

debugSymbolsAndLogs {
    message(Debug symbols and logging enabled.)
    CONFIG += force_debug_info
    DEFINES += ENABLE_DEBUG_LOGGING
}

warnings_as_errors {
    message(Warnings as errors enabled.)
}
