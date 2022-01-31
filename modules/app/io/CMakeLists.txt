cmake_minimum_required(VERSION 3.16)

project(tactile-app-io CXX)

add_subdirectory(maps)

set(TACTILE_IO_SOURCE_FILES

    ${PROJECT_SOURCE_DIR}/directories.cpp
    ${PROJECT_SOURCE_DIR}/directories.hpp
    ${PROJECT_SOURCE_DIR}/file_dialog.cpp
    ${PROJECT_SOURCE_DIR}/file_dialog.hpp

    ${PROJECT_SOURCE_DIR}/persistence/history.cpp
    ${PROJECT_SOURCE_DIR}/persistence/history.hpp
    ${PROJECT_SOURCE_DIR}/persistence/preference_state.cpp
    ${PROJECT_SOURCE_DIR}/persistence/preference_state.hpp
    ${PROJECT_SOURCE_DIR}/persistence/preferences.cpp
    ${PROJECT_SOURCE_DIR}/persistence/preferences.hpp
    ${PROJECT_SOURCE_DIR}/persistence/session.cpp
    ${PROJECT_SOURCE_DIR}/persistence/session.hpp

    ${TACTILE_IO_MAPS_SOURCE_FILES}

    PARENT_SCOPE)
