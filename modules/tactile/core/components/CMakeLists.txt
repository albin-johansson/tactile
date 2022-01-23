cmake_minimum_required(VERSION 3.16)

project(tactile-app-core-components CXX)

set(TACTILE_CORE_COMPONENT_SOURCE_FILES

    ${PROJECT_SOURCE_DIR}/animation.hpp
    ${PROJECT_SOURCE_DIR}/component.hpp
    ${PROJECT_SOURCE_DIR}/fancy_tile.hpp
    ${PROJECT_SOURCE_DIR}/layer.hpp
    ${PROJECT_SOURCE_DIR}/object.hpp
    ${PROJECT_SOURCE_DIR}/parent.hpp
    ${PROJECT_SOURCE_DIR}/property.hpp
    ${PROJECT_SOURCE_DIR}/property_context.hpp
    ${PROJECT_SOURCE_DIR}/texture.hpp
    ${PROJECT_SOURCE_DIR}/tileset.hpp
    ${PROJECT_SOURCE_DIR}/tool.hpp
    ${PROJECT_SOURCE_DIR}/uv_tile_size.hpp

    PARENT_SCOPE)