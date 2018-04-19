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
CONFIG   +=c++11

TEMPLATE = app


SOURCES += main.cpp \
    camera.cpp \
    motor.cpp \
    ledlattice.cpp \
    imageprocess.cpp \
    kinematiccontrol.cpp \
    utils.cpp \
    mpu/MPU6050.cpp \
    mpu/I2Cdev.cpp \
    mpu/demo_dmp.cpp \
    robotStatus.cpp

HEADERS += \
    camera.h \
    motor.h \
    ledlattice.h \
    imageprocess.h \
    kinematiccontrol.h \
    global_setting.h \
    utils.h \
    mpu/MPU6050.h \
    mpu/MPU6050_6Axis_MotionApps20.h \
    mpu/I2Cdev.h \
    mpu/demo_dmp.h \
    mpu/helper_3dmath.h \
    robotStatus.h

unix:!macx: LIBS += -lwiringPi


unix:!macx: LIBS += -L$$PWD/../../../../usr/local/lib/ -lopencv_core  -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc

INCLUDEPATH += $$PWD/../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../usr/local/include
