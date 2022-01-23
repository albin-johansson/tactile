cmake_minimum_required(VERSION 3.16)

project(tactile-app-editor-gui-menus CXX)

set(TACTILE_EDITOR_GUI_MENUS_SOURCE_FILES

    ${PROJECT_SOURCE_DIR}/debug_menu.cpp
    ${PROJECT_SOURCE_DIR}/debug_menu.hpp
    ${PROJECT_SOURCE_DIR}/edit_menu.cpp
    ${PROJECT_SOURCE_DIR}/edit_menu.hpp
    ${PROJECT_SOURCE_DIR}/file_menu.cpp
    ${PROJECT_SOURCE_DIR}/file_menu.hpp
    ${PROJECT_SOURCE_DIR}/help_menu.cpp
    ${PROJECT_SOURCE_DIR}/help_menu.hpp
    ${PROJECT_SOURCE_DIR}/map_menu.cpp
    ${PROJECT_SOURCE_DIR}/map_menu.hpp
    ${PROJECT_SOURCE_DIR}/menu_bar.cpp
    ${PROJECT_SOURCE_DIR}/menu_bar.hpp
    ${PROJECT_SOURCE_DIR}/view_menu.cpp
    ${PROJECT_SOURCE_DIR}/view_menu.hpp

    PARENT_SCOPE)