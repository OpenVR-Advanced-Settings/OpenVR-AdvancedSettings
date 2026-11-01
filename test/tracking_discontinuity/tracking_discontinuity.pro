QT += testlib
QT -= gui
CONFIG += c++1z console warn_on depend_includepath testcase
CONFIG -= app_bundle
TEMPLATE = app
INCLUDEPATH += ../../third-party/openvr/headers
SOURCES += tst_trackingdiscontinuity.cpp
HEADERS += ../../src/utils/TrackingDiscontinuity.h
win32-msvc {
    include(../../build_scripts/qt/compilers/common-msvc.pri)
    QMAKE_CXXFLAGS += /WX
}
