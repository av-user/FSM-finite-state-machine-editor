TEMPLATE = app

QT += qml quick widgets

CONFIG += c++11
CONFIG += object_parallel_to_source

SOURCES += main.cpp \
    handlers/Comm/Framework/Commhdl.cpp \
    handlers/Comm/Framework/StateComm.cpp \
    handlers/Main/Framework/Mainhdl.cpp \
    handlers/Main/Framework/StateMain.cpp \
    translationutil.cpp \
    handlers/Comm/Disconnected.cpp \
    handlers/Comm/Handshake.cpp \
    handlers/Comm/Idle.cpp \
    handlers/Comm/Response.cpp \
    handlers/Main/Ask.cpp \
    handlers/Main/Buy.cpp \
    handlers/Main/Error.cpp \
    handlers/Main/Idle.cpp \
    handlers/Main/Initialize.cpp \
    handlers/Main/Logon.cpp \
    handlers/Main/Show.cpp \
    statemodel/msg.cpp \
    statemodel/state.cpp \
    statemodel/timer.cpp \
    statemodel/timerthread.cpp \
    statemodel/handler.cpp \
    handlers/Comm/clientthread.cpp \
    handlers/Comm/tcpsocket.cpp \
    utilities/host/hosthelper.cpp

lupdate_only{
SOURCES = qml/main.qml \
            qml/start.qml \
            qml/userpass.qml \
            qml/signed.qml \
            qml/KeyboardNum.qml \
            qml/buy.qml \
            qml/question.qml \
            qml/show.qml \
}

TRANSLATIONS    =	./texts/sbmain_de.ts \
					./texts/sbmain_it.ts

RESOURCES += qml.qrc

STATECHANGE_DEBUG = 0

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    translationutil.h \
    handlers/Comm/Disconnected.h \
    handlers/Comm/Handshake.h \
    handlers/Comm/Idle.h \
    handlers/Comm/Response.h \
    handlers/Main/Ask.h \
    handlers/Main/Buy.h \
    handlers/Main/Error.h \
    handlers/Main/Idle.h \
    handlers/Main/Initialize.h \
    handlers/Main/Logon.h \
    handlers/Main/Show.h \
    handlers/Comm/Framework/Commhdl.h \
    handlers/Comm/Framework/StateComm.h \
    handlers/Main/Framework/Mainhdl.h \
    handlers/Main/Framework/StateMain.h \
    statemodel/handler.h \
    statemodel/msg.h \
    statemodel/state.h \
    statemodel/timer.h \
    statemodel/timerthread.h \
    handlers/Comm/tcpsocket.h \
    handlers/Comm/clientthread.h \
    utilities/host/hosthelper.h

DISTFILES += \
    cfg/log.cfg \
    handlers/Comm.fsm \
    handlers/Main.fsm

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-LogLib-Desktop_Qt_5_7_0_MSVC2015_64bit-Debug/release/ -lLogLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-LogLib-Desktop_Qt_5_7_0_MSVC2015_64bit-Debug/debug/ -lLogLib
else:unix: LIBS += -L$$PWD/../build-LogLib-Desktop_Qt_5_7_0_MSVC2015_64bit-Debug/ -lLogLib

INCLUDEPATH += $$PWD/../build-LogLib-Desktop_Qt_5_7_0_MSVC2015_64bit-Debug/debug
DEPENDPATH += $$PWD/../build-LogLib-Desktop_Qt_5_7_0_MSVC2015_64bit-Debug/debug

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-UtilLib-Desktop_Qt_5_7_0_MSVC2015_64bit-Debug/release/ -lUtilLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-UtilLib-Desktop_Qt_5_7_0_MSVC2015_64bit-Debug/debug/ -lUtilLib
else:unix: LIBS += -L$$PWD/../build-UtilLib-Desktop_Qt_5_7_0_MSVC2015_64bit-Debug/ -lUtilLib

INCLUDEPATH += $$PWD/../build-UtilLib-Desktop_Qt_5_7_0_MSVC2015_64bit-Debug/debug
DEPENDPATH += $$PWD/../build-UtilLib-Desktop_Qt_5_7_0_MSVC2015_64bit-Debug/debug
