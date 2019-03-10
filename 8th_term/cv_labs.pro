TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += $$(OPENCV)/install/include
LIBS += -L$$(OPENCV)/bin \
        -lopencv_core331 \
        -lopencv_imgcodecs331 \
        -lopencv_highgui331 \
        -lopencv_imgproc331 \
        -lopencv_videoio331

SOURCES += \
        main.cpp \
    6/lab6_class.cpp

HEADERS += \
    6/lab6_class.h
