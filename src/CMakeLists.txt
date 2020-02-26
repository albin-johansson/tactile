cmake_minimum_required(VERSION 3.12)
project(Tactile2-src)

set(TACTILE_SOURCE_FILES
        main.cpp
        tactile_window.h
        tactile_window.cpp)

add_executable(Tactile ${TACTILE_SOURCE_FILES})

target_link_libraries(Tactile
        Qt5::Core
        Qt5::Widgets
        Qt5::Gui)

copy_directory_post_build(Tactile "${TACTILE_ROOT_DIR}/bin" ${CMAKE_CURRENT_BINARY_DIR})