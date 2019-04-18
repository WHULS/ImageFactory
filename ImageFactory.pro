#-------------------------------------------------
#
# Project created by QtCreator 2019-03-22T22:17:19
#
#-------------------------------------------------

QT       += core gui xml

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
    myslider.cpp \
    houghcircledlg.cpp \
    CaliImage.cpp \
    ControlPointDlg.cpp \
    SelectCpDlg.cpp \
    SampleData.cpp \
    Matrix.cpp \
    FeatureWin.cpp \
    CalibrationWin.cpp

HEADERS += \
        mainwindow.h \
    mygraphicsview.h \
    convert.h \
    convert.h \
    myslider.h \
    houghcircledlg.h \
    CaliImage.h \
    ControlPointDlg.h \
    SelectCpDlg.h \
    SampleData.h \
    Matrix.h \
    FeatureWin.h \
    CalibrationWin.h

FORMS += \
    mainwindow.ui \
    houghcircledlg.ui \
    ControlPointDlg.ui \
    SelectCpDlg.ui \
    SampleData.ui \
    FeatureWin.ui \
    CalibrationWin.ui

UI_DIR = $$PWD

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc

DISTFILES += \
    icons.rc
RC_FILE += icons.rc

# OpenCV
INCLUDEPATH += $$PWD/cv/include
LIBS += -L $$PWD/cv/lib/libopencv_calib3d401.dll.a
LIBS += -L $$PWD/cv/lib/libopencv_*.a
#LIBS += -L $$PWD/cv/lib/libopencv_dnn401.dll.a
#LIBS += -L $$PWD/cv/lib/libopencv_features2d401.dll.a
#LIBS += -L $$PWD/cv/lib/libopencv_flann401.dll.a
#LIBS += -L $$PWD/cv/lib/libopencv_gapi401.dll.a
#LIBS += -L $$PWD/cv/lib/libopencv_highgui401.dll.a
#LIBS += -L $$PWD/cv/lib/libopencv_imgcodecs401.dll.a
#LIBS += -L $$PWD/cv/lib/libopencv_imgproc401.dll.a
#LIBS += -L $$PWD/cv/lib/libopencv_ml401.dll.a
#LIBS += -L $$PWD/cv/lib/libopencv_objdetect401.dll.a
#LIBS += -L $$PWD/cv/lib/libopencv_photo401.dll.a
#LIBS += -L $$PWD/cv/lib/libopencv_stitching401.dll.a
#LIBS += -L $$PWD/cv/lib/libopencv_video401.dll.a
#LIBS += -L $$PWD/cv/lib/libopencv_videoio401.dll.a
