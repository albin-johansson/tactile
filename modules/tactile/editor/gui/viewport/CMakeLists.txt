cmake_minimum_required(VERSION 3.16)

project(tactile-app-editor-gui-viewport CXX)

set(TACTILE_EDITOR_GUI_VIEWPORT_SOURCE_FILES

    ${PROJECT_SOURCE_DIR}/document_tabs.cpp
    ${PROJECT_SOURCE_DIR}/document_tabs.hpp
    ${PROJECT_SOURCE_DIR}/home_page_content.cpp
    ${PROJECT_SOURCE_DIR}/home_page_content.hpp
    ${PROJECT_SOURCE_DIR}/map_view.cpp
    ${PROJECT_SOURCE_DIR}/map_view.hpp
    ${PROJECT_SOURCE_DIR}/viewport_cursor_info.cpp
    ${PROJECT_SOURCE_DIR}/viewport_cursor_info.hpp
    ${PROJECT_SOURCE_DIR}/viewport_overlay.cpp
    ${PROJECT_SOURCE_DIR}/viewport_overlay.hpp
    ${PROJECT_SOURCE_DIR}/viewport_widget.cpp
    ${PROJECT_SOURCE_DIR}/viewport_widget.hpp

    PARENT_SCOPE)