#-------------------------------------------------
#
# Project created by QtCreator 2016-06-23T19:46:13
#
#-------------------------------------------------

QT       -= gui

TARGET = LogLib
TEMPLATE = lib

DEFINES += LOGLIB_LIBRARY

SOURCES += logger.cpp

HEADERS += logger.h\
        loglib_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
