QT += testlib
QT -= gui
CONFIG   += c++1z

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../../src/keyboard_input

SOURCES +=  tst_parsertest.cpp \
    ../../src/keyboard_input/input_parser.cpp

HEADERS += \
    ../../src/keyboard_input/input_parser.h
