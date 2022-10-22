cmake_minimum_required(VERSION 3.19)

option(QMODSORTER_BUILD_TESTS "Build tests" OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(qmodsorter LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

add_subdirectory(src)

if (QMODSORTER_BUILD_TESTS)
    include(CTest)
    add_subdirectory(tests)
endif()
