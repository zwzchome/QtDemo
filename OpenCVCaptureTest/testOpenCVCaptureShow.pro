#-------------------------------------------------
#
# Project created by QtCreator 2025-01-02T14:47:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = testOpenCVCaptureShow
TEMPLATE = app
DESTDIR = $$PWD/bin
# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

SOURCES += main.cpp\
        MainWindow.cpp \
    CameraCaptureThd.cpp \
    CameraWidget.cpp

HEADERS  += MainWindow.h \
    BaseThd.h \
    CameraCaptureThd.h \
    Singleton.h \
    CameraWidget.h

FORMS    += MainWindow.ui
INCLUDEPATH += $$PWD/lib/opencv/include

Debug:{
    LIBS += $$PWD/lib/opencv/lib/opencv_world455d.lib
}
Release:{
    LIBS += $$PWD/lib/opencv/lib/opencv_world455.lib
}
