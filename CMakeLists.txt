cmake_minimum_required(VERSION 3.12)

project(tactile)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(CMAKE_PREFIX_PATH "$ENV{BOOST_ROOT}; $ENV{Qt5_PATH}")

include(Utilities)

find_env_var(Qt5_PATH)
find_env_var(BOOST_ROOT)

foreach (path ${CMAKE_PREFIX_PATH})
    message("CMake prefix path: " ${path})
endforeach (path)

set(LIBRARY_DIR ${PROJECT_SOURCE_DIR}/lib)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

set(TACTILE_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

find_package(Qt5 COMPONENTS Core Widgets Gui REQUIRED)
#find_package(Boost 1.74.0 REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_subdirectory(src)
add_subdirectory(test)