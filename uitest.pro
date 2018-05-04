#-------------------------------------------------
#
# Project created by QtCreator 2018-05-02T22:28:30
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = uitest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    camera.cpp \
    formation_control.cpp \
    imageprocess.cpp \
    kinematiccontrol.cpp \
    ledlattice.cpp \
    motor.cpp \
    robotStatus.cpp \
    utils.cpp \
    mpu/demo_dmp.cpp \
    mpu/I2Cdev.cpp \
    mpu/MPU6050.cpp

HEADERS  += mainwindow.h \
    camera.h \
    formation_control.h \
    global_setting.h \
    imageprocess.h \
    kinematiccontrol.h \
    ledlattice.h \
    motor.h \
    robotStatus.h \
    utils.h \
    mpu/demo_dmp.h \
    mpu/helper_3dmath.h \
    mpu/I2Cdev.h \
    mpu/MPU6050.h \
    mpu/MPU6050_6Axis_MotionApps20.h

FORMS    += mainwindow.ui
INCLUDEPATH += /usr/local/include\
                /usr/local/include/opencv\
                /usr/local/include/opencv2\


LIBS += /usr/local/lib/libopencv_highgui.so\
        /usr/local/lib/libopencv_core.so\
        /usr/local/lib/libopencv_imgproc.so\
        /usr/local/lib/libopencv_calib3d.so\
        /usr/local/lib/libopencv_features2d.so\
        /usr/local/lib/libopencv_flann.so\
        /usr/local/lib/libopencv_imgcodecs.so\
        /usr/local/lib/libopencv_ml.so\
        /usr/local/lib/libopencv_objdetect.so\
        /usr/local/lib/libopencv_photo.so\
        /usr/local/lib/libopencv_shape.so\
        /usr/local/lib/libopencv_stitching.so\
        /usr/local/lib/libopencv_superres.so\
        /usr/local/lib/libopencv_video.so\
        /usr/local/lib/libopencv_videoio.so\
        /usr/local/lib/libopencv_videostab.so\
        /usr/local/lib/libARToolKitPlus.so

DISTFILES +=

RESOURCES +=

unix:!macx: LIBS += -lwiringPi


unix:!macx: LIBS += -L$$PWD/../../../../usr/local/lib/ -lopencv_core  -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc -lopencv_video -lopencv_videoio -lopencv_flann -lopencv_calib3d

INCLUDEPATH += $$PWD/../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../usr/local/include
