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
    pbuffer.cpp \
    resourceitemwidget.cpp \
    pdrawitemscontroller.cpp \
    qdrawablelistwidget.cpp \
    rangeparameterwidget.cpp \
    qparameterscontroller.cpp \
    rangeparametervariancewidget.cpp

HEADERS  += mainwindow.h \
    qmainglwidget.h \
    qmymenubar.h \
    pdrawableitem.h \
    pparticlesystem.h \
    pbuffer.h \
    resourceitemwidget.h \
    pdrawitemscontroller.h \
    qdrawablelistwidget.h \
    rangeparameterwidget.h \
    qparameterscontroller.h \
    rangeparametervariancewidget.h

FORMS    += mainwindow.ui

RESOURCES += \
    icons.qrc \
    psystem.qrc

DISTFILES += \
    shaders/particle.frag \
    shaders/particle.vert \
    particles/ball.png
