#-------------------------------------------------
#
# Project created by QtCreator 2016-06-09T20:06:46
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HostSim
TEMPLATE = app


SOURCES += main.cpp\
	mainwindow.cpp \
    hostserver.cpp \
    hostthread.cpp \
    tcpclient.cpp \
    utility.cpp

HEADERS  += mainwindow.h \
    hostserver.h \
    hostthread.h \
    tcpclient.h \
    utility.h

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-LogLib-Desktop_Qt_5_7_0_MSVC2015_64bit-Debug/release/ -lLogLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-LogLib-Desktop_Qt_5_7_0_MSVC2015_64bit-Debug/debug/ -lLogLib
else:unix: LIBS += -L$$PWD/../build-LogLib-Desktop_Qt_5_7_0_MSVC2015_64bit-Debug/ -lLogLib

INCLUDEPATH += $$PWD/../build-LogLib-Desktop_Qt_5_7_0_MSVC2015_64bit-Debug/debug
DEPENDPATH += $$PWD/../build-LogLib-Desktop_Qt_5_7_0_MSVC2015_64bit-Debug/debug
