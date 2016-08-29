#-------------------------------------------------
#
# Project created by QtCreator 2016-08-25T16:17:17
#
#-------------------------------------------------

QT       -= gui

TARGET = UtilLib
TEMPLATE = lib

DEFINES += UTILLIB_LIBRARY

SOURCES += reg.cpp \
    util.cpp

HEADERS += util.h reg.h\
        utillib_global.h \

unix {
    target.path = /usr/lib
    INSTALLS += target
}
