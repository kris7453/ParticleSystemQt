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
    qmainglwidget.cpp \
    qmymenubar.cpp \
    pparticlesystem.cpp \
    pdrawableitem.cpp \
    pbuffer.cpp

HEADERS  += mainwindow.h \
    qmainglwidget.h \
    psystemdata.h \
    qmymenubar.h \
    pdrawableitem.h \
    pparticlesystem.h \
    pbuffer.h

FORMS    += mainwindow.ui

RESOURCES += \
    icons.qrc \
    psystem.qrc

DISTFILES += \
    shaders/particle.frag \
    shaders/particle.vert \
    particles/ball.png \
    particles/ball2.png \
    particles/ball3.png
