cmake_minimum_required(VERSION 3.16)

project(tactile-app-editor-gui-toolbar CXX)

set(TACTILE_EDITOR_GUI_TOOLBAR_SOURCE_FILES

    ${PROJECT_SOURCE_DIR}/toolbar.cpp
    ${PROJECT_SOURCE_DIR}/toolbar.hpp
    ${PROJECT_SOURCE_DIR}/tool_button.cpp
    ${PROJECT_SOURCE_DIR}/tool_button.hpp

    PARENT_SCOPE)