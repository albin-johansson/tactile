cmake_minimum_required(VERSION 3.16)

project(tactile-app-editor-gui-common CXX)

set(TACTILE_EDITOR_GUI_COMMON_SOURCE_FILES

    ${PROJECT_SOURCE_DIR}/button.cpp
    ${PROJECT_SOURCE_DIR}/button.hpp
    ${PROJECT_SOURCE_DIR}/centered_text.cpp
    ${PROJECT_SOURCE_DIR}/centered_text.hpp
    ${PROJECT_SOURCE_DIR}/checkbox.cpp
    ${PROJECT_SOURCE_DIR}/checkbox.hpp
    ${PROJECT_SOURCE_DIR}/context_menu.cpp
    ${PROJECT_SOURCE_DIR}/context_menu.hpp
    ${PROJECT_SOURCE_DIR}/docking_toolbar.cpp
    ${PROJECT_SOURCE_DIR}/docking_toolbar.hpp
    ${PROJECT_SOURCE_DIR}/input_widgets.cpp
    ${PROJECT_SOURCE_DIR}/input_widgets.hpp
    ${PROJECT_SOURCE_DIR}/mouse_tracker.cpp
    ${PROJECT_SOURCE_DIR}/mouse_tracker.hpp
    ${PROJECT_SOURCE_DIR}/rubber_band.cpp
    ${PROJECT_SOURCE_DIR}/rubber_band.hpp

    PARENT_SCOPE)