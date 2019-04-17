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
        -lopencv_videoio331 \
        -lopencv_aruco331 \
        -lopencv_calib3d331

SOURCES += \
        main.cpp \

# lab6
HEADERS += \
    6/lab6_class.h \

SOURCES += \
    6/lab6_class.cpp \
    6/lab6_class_coins.cpp \
    6/lines.cpp \
#      6/lab6_class_thrash.cpp

#lab5
HEADERS += \
    5/lab5_class.h \
    5/lab5.h \

SOURCES += \
    5/lab5_class.cpp \
    5/lab5.cpp \

#Aruco
HEADERS += \
    Aruco/lab_aruco.h \
    Aruco/samples.hpp

SOURCES += \
    Aruco/lab_aruco.cpp \
    Aruco/draw_cube.cpp \
    Aruco/detect_markers.cpp \
#    Aruco/create_marker.cpp \
    Aruco/calibrate_camera.cpp \
    Aruco/aruco_lab.cpp \
    Aruco/completeArchivArucoLab/myDetectionsCube.cpp \
