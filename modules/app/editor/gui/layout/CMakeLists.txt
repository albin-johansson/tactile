cmake_minimum_required(VERSION 3.16)

project(tactile-app-editor-gui-layout CXX)

set(TACTILE_EDITOR_GUI_LAYOUT_SOURCE_FILES

    ${PROJECT_SOURCE_DIR}/dock_space.cpp
    ${PROJECT_SOURCE_DIR}/dock_space.hpp
    ${PROJECT_SOURCE_DIR}/load_default_layout.cpp
    ${PROJECT_SOURCE_DIR}/load_default_layout.hpp

    PARENT_SCOPE)