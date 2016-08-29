#-------------------------------------------------
#
# Project created by QtCreator 2016-02-24T14:03:19
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TestScene
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    scene.cpp \
    itemdialog.cpp \
    backbone.cpp \
    eventitem.cpp \
    transition.cpp \
    eventdialog.cpp \
    statedialog.cpp \
    statetem.cpp

HEADERS  += dialog.h \
    scene.h \
    itemdialog.h \
    backbone.h \
    eventitem.h \
    transition.h \
    eventdialog.h \
    statedialog.h \
    stateitem.h

FORMS    += dialog.ui \
    itemdialog.ui \
    eventdialog.ui \
    statedialog.ui

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

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-CodeGen-Desktop_Qt_5_7_0_MSVC2015_64bit-Debug/release/ -lCodeGen
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-CodeGen-Desktop_Qt_5_7_0_MSVC2015_64bit-Debug/debug/ -lCodeGen
else:unix: LIBS += -L$$PWD/../build-CodeGen-Desktop_Qt_5_7_0_MSVC2015_64bit-Debug/ -lCodeGen

INCLUDEPATH += $$PWD/../build-CodeGen-Desktop_Qt_5_7_0_MSVC2015_64bit-Debug/debug
DEPENDPATH += $$PWD/../build-CodeGen-Desktop_Qt_5_7_0_MSVC2015_64bit-Debug/debug
