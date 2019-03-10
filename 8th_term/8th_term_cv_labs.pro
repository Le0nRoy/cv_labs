#-------------------------------------------------
#
# Project created by QtCreator 2017-03-05T12:30:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = opencvtest
TEMPLATE = app

CONFIG += console c++11

INCLUDEPATH += $$(OPENCV)/install/include
LIBS += -L$$(OPENCV)/bin #\
#        -lopencv_core331 \
#        -lopencv_imgcodecs331 \
#        -lopencv_highgui331

HEADERS += 6/lab6_class.h

SOURCES += main.cpp
SOURCES += 6/lab6_class.cpp
