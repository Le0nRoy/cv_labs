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
    6/lab6_class.cpp \
    6/lab6_class_lines.cpp \
    6/lab6_class_coins.cpp \
    5/lab5_class.cpp \
    Aruco/lab_aruco.cpp
#      6/lab6_class_thrash.cpp

HEADERS += \
    6/lab6_class.h \
    5/lab5_class.h \
    Aruco/lab_aruco.h
