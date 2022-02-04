cmake_minimum_required(VERSION 3.16)

project(tactile-app-editor-gui-tilesets CXX)

set(TACTILE_EDITOR_GUI_TILESETS_SOURCE_FILES

    ${PROJECT_SOURCE_DIR}/tileset_dock.cpp
    ${PROJECT_SOURCE_DIR}/tileset_dock.hpp
    ${PROJECT_SOURCE_DIR}/tileset_tabs.cpp
    ${PROJECT_SOURCE_DIR}/tileset_tabs.hpp
    ${PROJECT_SOURCE_DIR}/tileset_view.cpp
    ${PROJECT_SOURCE_DIR}/tileset_view.hpp

    ${PROJECT_SOURCE_DIR}/dialogs/create_tileset_dialog.cpp
    ${PROJECT_SOURCE_DIR}/dialogs/create_tileset_dialog.hpp

    PARENT_SCOPE)