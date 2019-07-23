QT       += core gui qml quick multimedia widgets
CONFIG   += c++1z file_copies

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
COPIES += resCopy readmeCopy licenseCopy packageFilesCopy openvrApiCopy
COPY_DEST_DIR = $$OUT_PWD/$$DESTDIR

resCopy.files = src/res/*
resCopy.path = $$COPY_DEST_DIR/res

readmeCopy.files = Readme.md
readmeCopy.path = $$COPY_DEST_DIR

licenseCopy.files = LICENSE \
                    third-party/openvr/LICENSE-VALVE  \
                    third-party/easylogging++/LICENSE-MIT
licenseCopy.path = $$COPY_DEST_DIR

packageFilesCopy.files = src/package_files/*
packageFilesCopy.path = $$COPY_DEST_DIR

win32:openvrApiCopy.files = third-party/openvr/bin/win64/openvr_api.dll
unix:openvrApiCopy.files = third-party/openvr/lib/linux64/libopenvr_api.so
openvrApiCopy.path = $$COPY_DEST_DIR
