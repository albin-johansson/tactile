cmake_minimum_required(VERSION 3.16)

project(tactile-app-io CXX)

add_subdirectory(maps)

set(TACTILE_IO_SOURCE_FILES

    ${PROJECT_SOURCE_DIR}/directories.cpp
    ${PROJECT_SOURCE_DIR}/directories.hpp
    ${PROJECT_SOURCE_DIR}/file_dialog.cpp
    ${PROJECT_SOURCE_DIR}/file_dialog.hpp
    ${PROJECT_SOURCE_DIR}/history.cpp
    ${PROJECT_SOURCE_DIR}/history.hpp
    ${PROJECT_SOURCE_DIR}/preferences.cpp
    ${PROJECT_SOURCE_DIR}/preferences.hpp
    ${PROJECT_SOURCE_DIR}/session.cpp
    ${PROJECT_SOURCE_DIR}/session.hpp

    ${TACTILE_IO_MAPS_SOURCE_FILES}

    PARENT_SCOPE)
