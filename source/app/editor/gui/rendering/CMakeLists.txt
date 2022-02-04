cmake_minimum_required(VERSION 3.16)

project(tactile-app-editor-gui-rendering CXX)

set(TACTILE_EDITOR_GUI_RENDERING_SOURCE_FILES

    ${PROJECT_SOURCE_DIR}/graphics.cpp
    ${PROJECT_SOURCE_DIR}/graphics.hpp
    ${PROJECT_SOURCE_DIR}/render_bounds.cpp
    ${PROJECT_SOURCE_DIR}/render_bounds.hpp
    ${PROJECT_SOURCE_DIR}/render_info.cpp
    ${PROJECT_SOURCE_DIR}/render_info.hpp
    ${PROJECT_SOURCE_DIR}/render_map.cpp
    ${PROJECT_SOURCE_DIR}/render_map.hpp
    ${PROJECT_SOURCE_DIR}/render_object_layer.cpp
    ${PROJECT_SOURCE_DIR}/render_object_layer.hpp
    ${PROJECT_SOURCE_DIR}/render_stamp_preview.cpp
    ${PROJECT_SOURCE_DIR}/render_stamp_preview.hpp
    ${PROJECT_SOURCE_DIR}/render_tile.cpp
    ${PROJECT_SOURCE_DIR}/render_tile.hpp
    ${PROJECT_SOURCE_DIR}/render_tile_layer.cpp
    ${PROJECT_SOURCE_DIR}/render_tile_layer.hpp

    PARENT_SCOPE)