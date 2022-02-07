cmake_minimum_required(VERSION 3.16)

project(tactile-app-core CXX)

add_subdirectory(algorithms)
add_subdirectory(components)
add_subdirectory(systems)
add_subdirectory(utils)

set(TACTILE_CORE_SOURCE_FILES

    ${PROJECT_SOURCE_DIR}/attribute.cpp
    ${PROJECT_SOURCE_DIR}/attribute.hpp
    ${PROJECT_SOURCE_DIR}/layer_type.hpp
    ${PROJECT_SOURCE_DIR}/map.hpp
    ${PROJECT_SOURCE_DIR}/mouse_info.hpp
    ${PROJECT_SOURCE_DIR}/object_type.hpp
    ${PROJECT_SOURCE_DIR}/region.hpp
    ${PROJECT_SOURCE_DIR}/tile_cache.hpp
    ${PROJECT_SOURCE_DIR}/tile_position.hpp
    ${PROJECT_SOURCE_DIR}/tool_type.hpp
    ${PROJECT_SOURCE_DIR}/viewport.hpp

    ${TACTILE_CORE_ALGORITHM_SOURCE_FILES}
    ${TACTILE_CORE_COMPONENT_SOURCE_FILES}
    ${TACTILE_CORE_SYSTEM_SOURCE_FILES}
    ${TACTILE_CORE_UTILS_SOURCE_FILES}

    PARENT_SCOPE)
