cmake_minimum_required(VERSION 3.12)
project(tactile)

# NOTE: If the build fails with MinGW, try setting the QT_QPA_PLATFORM_PLUGIN_PATH
# environment variable to the path of the "plugins/platforms" directory in the
# current Qt compiler directory.

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

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(CMAKE_PREFIX_PATH $ENV{Qt5_PATH})

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

add_subdirectory(src)

enable_testing()
add_subdirectory(test)