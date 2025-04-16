cmake_minimum_required(VERSION 3.5)

project(ClipTrack)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    add_compile_options(/Zc:__cplusplus /permissive-)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(ClipTrack WIN32 ClipTrack.cpp main.cpp resources.qrc ClipTrack.rc)

target_link_libraries(ClipTrack Qt6::Widgets)