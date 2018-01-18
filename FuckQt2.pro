#-------------------------------------------------
#
# Project created by QtCreator 2018-01-06T20:48:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
LIBS += -lws2_32
TARGET = FuckQt2
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    connectwaitdlg.cpp \
    basesock.cpp \
    sockdata.cpp\
    sockthread.cpp \
    connthread.cpp \
    rpsdlg.cpp \
    common.cpp


HEADERS  += mainwindow.h \
    connectwaitdlg.h \
    basesock.h \
    sockdata.h \
    sockthread.h \
    connthread.h \
    common.h \
    rpsdlg.h \
    headers.h
FORMS += mainwindow.ui \
    connectwaitdlg.ui \
    rpsdlg.ui

