cmake_minimum_required(VERSION 3.12)

project(tactile)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(CMAKE_PREFIX_PATH $ENV{Qt5_PATH})
set(LIBRARY_DIR ${PROJECT_SOURCE_DIR}/lib)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

set(TACTILE_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

if (NOT DEFINED ENV{Qt5_PATH})
    message(WARNING "Qt5_PATH is not a defined environment variable!")
else ()
    message("Qt5 path: $ENV{Qt5_PATH}\ ")
endif ()

find_package(Qt5 COMPONENTS Core Widgets Gui REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Copies a directory.
#   target: the associated target.
#   from: the directory that will be copied.
#   to: the target destination of the copied directory.
function(copy_directory_post_build [target [from [to]]])
    add_custom_command(
            TARGET ${ARGV0} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${ARGV1}
            ${ARGV2})
endfunction()

# Creates an interface library target for a header-only library.
#   name: the name of the library target.
#   includeDirectory: the path of the directory that contains the headers of the library.
#   sources: the headers associated with the library.
function(add_header_only_lib [name [includeDirectory [sources]]])
    add_library(${ARGV0} INTERFACE)
    target_sources(${ARGV0} INTERFACE ${ARGV2})
    target_include_directories(${ARGV0} SYSTEM INTERFACE ${ARGV1})
endfunction()

add_subdirectory(src)
add_subdirectory(test)