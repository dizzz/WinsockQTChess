#-------------------------------------------------
#
# Project created by QtCreator 2018-01-19T12:58:50
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WinsockQTChess
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_MESSAGELOGCONTEXT
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += /source
SOURCES += main.cpp\
        mainwindow.cpp \
    tile.cpp \
    validation.cpp\
    connectwaitdlg.cpp \
    basesock.cpp \
    sockdata.cpp\
    sockthread.cpp \
    rpsdlg.cpp \
    common.cpp \
    game.cpp


HEADERS  += mainwindow.h \
    tile.h \
    validation.h\
    connectwaitdlg.h \
    basesock.h \
    sockdata.h \
    sockthread.h \
    common.h \
    rpsdlg.h \
    game.h

FORMS    += mainwindow.ui\
    connectwaitdlg.ui \
    rpsdlg.ui

RESOURCES += \
    Images.qrc



