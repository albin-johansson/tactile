cmake_minimum_required(VERSION 3.16)

project(tactile-app-editor-shortcuts CXX)

set(TACTILE_EDITOR_SHORTCUT_SOURCE_FILES

    ${PROJECT_SOURCE_DIR}/edit_shortcuts.cpp
    ${PROJECT_SOURCE_DIR}/edit_shortcuts.hpp
    ${PROJECT_SOURCE_DIR}/file_shortcuts.cpp
    ${PROJECT_SOURCE_DIR}/file_shortcuts.hpp
    ${PROJECT_SOURCE_DIR}/mappings.hpp
    ${PROJECT_SOURCE_DIR}/shortcut.cpp
    ${PROJECT_SOURCE_DIR}/shortcut.hpp
    ${PROJECT_SOURCE_DIR}/shortcuts.cpp
    ${PROJECT_SOURCE_DIR}/shortcuts.hpp
    ${PROJECT_SOURCE_DIR}/view_shortcuts.cpp
    ${PROJECT_SOURCE_DIR}/view_shortcuts.hpp

    PARENT_SCOPE)