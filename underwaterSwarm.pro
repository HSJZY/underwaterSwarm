#-------------------------------------------------
#
# Project created by QtCreator 2018-03-19T17:34:19
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = underwaterSwarm
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    camera.cpp \
    motor.cpp \
    ledlattice.cpp \
    imageprocess.cpp \
    kinematiccontrol.cpp

HEADERS += \
    camera.h \
    motor.h \
    ledlattice.h \
    imageprocess.h \
    kinematiccontrol.h
