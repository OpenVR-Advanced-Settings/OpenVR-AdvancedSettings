QT       += core gui qml quick multimedia widgets
CONFIG   += c++1z

DEFINES += ELPP_THREAD_SAFE ELPP_QT_LOGGING ELPP_NO_DEFAULT_LOG_FILE

lessThan(QT_MAJOR_VERSION, 5): error("requires Qt 5.6 or higher")
lessThan(QT_MINOR_VERSION, 6): error("requires Qt 5.6 or higher")

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
