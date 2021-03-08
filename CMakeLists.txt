cmake_minimum_required(VERSION 3.12)

project(tactile CXX)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
set(CMAKE_PREFIX_PATH "$ENV{Qt6_PATH}")
set(CMAKE_CXX_STANDARD 20)

include(Utilities)

if (WIN32)
  find_env_var(Qt6_PATH)
endif ()

set(LIBRARY_DIR ${PROJECT_SOURCE_DIR}/lib)
set(TACTILE_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

find_package(Qt6 COMPONENTS Core Widgets OpenGLWidgets Gui Xml REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_subdirectory(src)
add_subdirectory(test)