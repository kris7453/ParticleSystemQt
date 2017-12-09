#-------------------------------------------------
#
# Project created by QtCreator 2017-11-21T23:34:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PSystems
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    psystem.cpp \
    qmainglwidget.cpp \
    qmymenubar.cpp

HEADERS  += mainwindow.h \
    psystem.h \
    qmainglwidget.h \
    psystemdata.h \
    qmymenubar.h

FORMS    += mainwindow.ui

RESOURCES += \
    icons.qrc \
    psystem.qrc

DISTFILES += \
    shaders/particle.frag \
    shaders/particle.vert
