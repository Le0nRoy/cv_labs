cmake_minimum_required(VERSION 3.12)
project(lab3)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV REQUIRED)

add_executable(lab3
        main.cpp
#        1/lab1.cpp
#        2/lab2.cpp
#        2/lab2.h
#        3/lab3.cpp
#        3/lab3.h  
        4/lab4.cpp
        4/lab4.h
        )

target_link_libraries(lab3 ${OpenCV_LIBS})