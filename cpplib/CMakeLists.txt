cmake_minimum_required(VERSION 3.13)

project(PyCadCpp)

find_package(OpenCASCADE 7.3.0 REQUIRED)
find_package(PythonInterp 3.7 REQUIRED)
find_package(PythonLibs 3.7 REQUIRED)
find_package(Qt5 COMPONENTS Widgets OpenGL REQUIRED)

find_package(OpenGL)
include_directories(${OPENGL_INCLUDE_DIRS})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

file(GLOB FILES_CC "*.cc" "base/*.cc" "gen/*.cc" "viewer/*.cc" "py/out/*.cpp")

add_library(PyCadCpp SHARED ${FILES_CC})

target_include_directories(PyCadCpp PRIVATE ".")

# SIP related stuff
target_include_directories(PyCadCpp PRIVATE "/usr/include/python3.7m")

# OpenCASCADE include and libs
target_include_directories(PyCadCpp PRIVATE ${OpenCASCADE_INCLUDE_DIR})
target_link_libraries(PyCadCpp ${OpenCASCADE_LIBRARIES})

# qt stuff
include_directories(PyCadCpp ${Qt5Widgets_INCLUDE_DIRS} ${Qt5OpenGL_INCLUDE_DIRS})
target_link_libraries(PyCadCpp Qt5::Widgets Qt5::OpenGL)
