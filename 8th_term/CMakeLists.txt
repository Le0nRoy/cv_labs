cmake_minimum_required ( VERSION 3.13 )
project ( 8th_term )

set ( CMAKE_CXX_STANDARD 14 )

find_package ( OpenCV REQUIRED )

set ( LABS_SRC
      main.cpp
      6/lab6_class.cpp
      )

add_executable ( 8th_term
                 ${LABS_SRC}
                 )