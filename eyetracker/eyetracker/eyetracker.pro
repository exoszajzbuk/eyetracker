#-------------------------------------------------
#
# Project created by QtCreator 2013-04-22T10:58:01
#
#-------------------------------------------------

QT       += core gui

TARGET = eyetracker
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    videohandler.cpp \
    imageprocessor.cpp \
    calibrator.cpp \
    calibrationwindow.cpp

HEADERS  += mainwindow.h \
    videohandler.h \
    imageprocessor.h \
    calibrator.h \
    calibrationwindow.h

FORMS    += mainwindow.ui \
    calibrationwindow.ui

INCLUDEPATH += D:/Programok/OpenCV-2.3.0_build/install/include
LIBS += D:/Programok/OpenCV-2.3.0_build/install/bin/*.dll

QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3

RESOURCES += \
    icons.qrc
