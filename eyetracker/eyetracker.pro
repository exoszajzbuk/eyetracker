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
    calibrationwindow.cpp \
    sessionitemwidget.cpp \
    recorder.cpp \
    recordingwindow.cpp \
    session.cpp \
    helper.cpp \
    player.cpp \
    playerwindow.cpp \
    heatmapperwindow.cpp \
    heatmapper.cpp

HEADERS  += mainwindow.h \
    videohandler.h \
    imageprocessor.h \
    calibrator.h \
    calibrationwindow.h \
    sessionitemwidget.h \
    recorder.h \
    recordingwindow.h \
    session.h \
    helper.h \
    player.h \
    playerwindow.h \
    heatmapperwindow.h \
    heatmapper.h

FORMS    += mainwindow.ui \
    calibrationwindow.ui \
    sessionitemwidget.ui \
    recordingwindow.ui \
    playerwindow.ui \
    heatmapperwindow.ui

INCLUDEPATH += D:/Programok/OpenCV-2.3.0_build/install/include
LIBS += D:/Programok/OpenCV-2.3.0_build/install/bin/*.dll

QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3

RESOURCES += \
    icons.qrc
