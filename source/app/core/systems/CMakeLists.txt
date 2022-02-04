cmake_minimum_required(VERSION 3.16)

project(tactile-app-core-systems CXX)

set(TACTILE_CORE_SYSTEM_SOURCE_FILES

    ${PROJECT_SOURCE_DIR}/animation_system.cpp
    ${PROJECT_SOURCE_DIR}/animation_system.hpp
    ${PROJECT_SOURCE_DIR}/component_system.cpp
    ${PROJECT_SOURCE_DIR}/component_system.hpp
    ${PROJECT_SOURCE_DIR}/context_system.cpp
    ${PROJECT_SOURCE_DIR}/context_system.hpp
    ${PROJECT_SOURCE_DIR}/duplicate_comp.hpp
    ${PROJECT_SOURCE_DIR}/map_system.cpp
    ${PROJECT_SOURCE_DIR}/map_system.hpp
    ${PROJECT_SOURCE_DIR}/object_system.cpp
    ${PROJECT_SOURCE_DIR}/object_system.hpp
    ${PROJECT_SOURCE_DIR}/property_system.cpp
    ${PROJECT_SOURCE_DIR}/property_system.hpp
    ${PROJECT_SOURCE_DIR}/registry_system.cpp
    ${PROJECT_SOURCE_DIR}/registry_system.hpp
    ${PROJECT_SOURCE_DIR}/snapshot.hpp
    ${PROJECT_SOURCE_DIR}/tileset_system.cpp
    ${PROJECT_SOURCE_DIR}/tileset_system.hpp
    ${PROJECT_SOURCE_DIR}/viewport_system.cpp
    ${PROJECT_SOURCE_DIR}/viewport_system.hpp

    ${PROJECT_SOURCE_DIR}/layers/layer_snapshot.hpp
    ${PROJECT_SOURCE_DIR}/layers/layer_system.cpp
    ${PROJECT_SOURCE_DIR}/layers/layer_system.hpp
    ${PROJECT_SOURCE_DIR}/layers/layer_tree_system.cpp
    ${PROJECT_SOURCE_DIR}/layers/layer_tree_system.hpp
    ${PROJECT_SOURCE_DIR}/layers/object_layer_system.cpp
    ${PROJECT_SOURCE_DIR}/layers/object_layer_system.hpp
    ${PROJECT_SOURCE_DIR}/layers/tile_layer_system.cpp
    ${PROJECT_SOURCE_DIR}/layers/tile_layer_system.hpp

    ${PROJECT_SOURCE_DIR}/tools/bucket_tool_system.cpp
    ${PROJECT_SOURCE_DIR}/tools/bucket_tool_system.hpp
    ${PROJECT_SOURCE_DIR}/tools/eraser_tool_system.cpp
    ${PROJECT_SOURCE_DIR}/tools/eraser_tool_system.hpp
    ${PROJECT_SOURCE_DIR}/tools/object_selection_tool_system.cpp
    ${PROJECT_SOURCE_DIR}/tools/object_selection_tool_system.hpp
    ${PROJECT_SOURCE_DIR}/tools/stamp_tool_system.cpp
    ${PROJECT_SOURCE_DIR}/tools/stamp_tool_system.hpp
    ${PROJECT_SOURCE_DIR}/tools/tool_system.cpp
    ${PROJECT_SOURCE_DIR}/tools/tool_system.hpp

    PARENT_SCOPE)