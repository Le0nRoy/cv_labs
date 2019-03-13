cmake_minimum_required ( VERSION 3.13 )

set ( PROJECT_NAME 8th_term )

project ( ${PROJECT_NAME} )

set ( CMAKE_CXX_STANDARD 11 )

find_package ( OpenCV 3.3.1 REQUIRED )

set ( LABS_SRC
      main.cpp
      6/lab6_class.cpp
      6/lab6_class_lines.cpp
      6/lab6_class_coins.cpp
      )

add_executable ( 8th_term
                 ${LABS_SRC}
                 )

target_link_libraries ( ${PROJECT_NAME} ${OpenCV_LIBS} )
