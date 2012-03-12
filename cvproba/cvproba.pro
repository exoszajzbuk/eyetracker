#-------------------------------------------------
#
# Project created by QtCreator 2012-03-04T14:05:46
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = cvproba
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

INCLUDEPATH += D:/Programok/OpenCV-2.3.0_build/install/include
LIBS += D:/Programok/OpenCV-2.3.0_build/install/bin/*.dll

QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3
