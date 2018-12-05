cmake_minimum_required(VERSION 3.12)
project(CV_labs)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV REQUIRED)

add_executable(CV_labs
        main.cpp
        1/lab1.cpp
        1/lab1.h
        2/lab2.cpp
        2/lab2.h
        3/lab3.cpp
        3/lab3.h
        4/lab4.cpp
        4/lab4.h
        # Just for keeping bracket on another string
        )

target_link_libraries(CV_labs ${OpenCV_LIBS})