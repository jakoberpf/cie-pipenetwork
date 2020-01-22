cmake_minimum_required(VERSION 3.15)
project(linalg)

set(CMAKE_CXX_STANDARD 14)

include_directories(drivers)
include_directories(external)
include_directories(library)

add_executable(linalg
        library/inc/linalg.hpp
        library/inc/linalg_impl.hpp
        library/src/linalg.cpp

        library/test/linalg_test.cpp
        library/test/pipenetwork_test.cpp
        library/test/main_test.cpp library/src/Node.cpp library/inc/Node.hpp library/src/Tube.cpp library/inc/Tube.hpp library/src/PipeNetwork.cpp library/inc/PipeNetwork.hpp library/src/Parameters.cpp library/inc/Parameters.hpp)
