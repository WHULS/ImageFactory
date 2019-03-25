#-------------------------------------------------
#
# Project created by QtCreator 2019-03-22T22:17:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImageFactory
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

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    mygraphicsview.cpp \
    convert.cpp \
    myslider.cpp

HEADERS += \
        mainwindow.h \
    mygraphicsview.h \
    convert.h \
    convert.h \
    myslider.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc

DISTFILES += \
    icons.rc
RC_FILE += icons.rc

INCLUDEPATH += $$PWD/cv/include
LIBS += -L $$PWD/cv/lib/libopencv_*.a

#LIBS += E:\opencv-build\bin\libopencv_calib3d401.dll
#LIBS += E:\opencv-build\bin\libopencv_core401.dll
#LIBS += E:\opencv-build\bin\libopencv_dnn401.dll
#LIBS += E:\opencv-build\bin\libopencv_features2d401.dll
#LIBS += E:\opencv-build\bin\libopencv_flann401.dll
#LIBS += E:\opencv-build\bin\libopencv_gapi401.dll
#LIBS += E:\opencv-build\bin\libopencv_highgui401.dll
#LIBS += E:\opencv-build\bin\libopencv_imgcodecs401.dll
#LIBS += E:\opencv-build\bin\libopencv_imgproc401.dll
#LIBS += E:\opencv-build\bin\libopencv_ml401.dll
#LIBS += E:\opencv-build\bin\libopencv_objdetect401.dll
#LIBS += E:\opencv-build\bin\libopencv_photo401.dll
#LIBS += E:\opencv-build\bin\libopencv_stitching401.dll
#LIBS += E:\opencv-build\bin\libopencv_video401.dll
#LIBS += E:\opencv-build\bin\libopencv_videoio401.dll
#LIBS += E:\opencv-build\bin\opencv_ffmpeg401_64.dll

#LIBS += $$PWD\cv\lib\libopencv_calib3d401.dll.a
#LIBS += $$PWD\cv\lib\libopencv_core401.dll.a
#LIBS += $$PWD\cv\lib\libopencv_dnn401.dll.a
#LIBS += $$PWD\cv\lib\libopencv_features2d401.dll.a
#LIBS += $$PWD\cv\lib\libopencv_flann401.dll.a
#LIBS += $$PWD\cv\lib\libopencv_gapi401.dll.a
#LIBS += $$PWD\cv\lib\libopencv_highgui401.dll.a
#LIBS += $$PWD\cv\lib\libopencv_imgcodecs401.dll.a
#LIBS += $$PWD\cv\lib\libopencv_imgproc401.dll.a
#LIBS += $$PWD\cv\lib\libopencv_ml401.dll.a
#LIBS += $$PWD\cv\lib\libopencv_objdetect401.dll.a
#LIBS += $$PWD\cv\lib\libopencv_photo401.dll.a
#LIBS += $$PWD\cv\lib\libopencv_stitching401.dll.a
#LIBS += $$PWD\cv\lib\libopencv_video401.dll.a
#LIBS += $$PWD\cv\lib\libopencv_videoio401.dll.a
