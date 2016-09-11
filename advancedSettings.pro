#-------------------------------------------------
#
# Project created by QtCreator 2015-06-10T16:57:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AdvancedSettings
TEMPLATE = app


SOURCES += src/main.cpp\
        src/overlaywidget.cpp \
		src/overlaycontroller.cpp 

HEADERS  += src/overlaywidget.h \
		src/overlaycontroller.h
		src/logging.h

FORMS    += ui/overlaywidget.ui

INCLUDEPATH += third-party/openvr/include \
			third-party/easylogging++

LIBS += -Lthird-party/openvr/lib/win64 -lopenvr_api

DESTDIR = bin/win64
