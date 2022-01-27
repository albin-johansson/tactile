cmake_minimum_required(VERSION 3.16)

project(tactile-app-core-utils CXX)

set(TACTILE_CORE_UTILS_SOURCE_FILES

    ${PROJECT_SOURCE_DIR}/buffer_utils.hpp
    ${PROJECT_SOURCE_DIR}/colors.hpp
    ${PROJECT_SOURCE_DIR}/formatted_string.hpp
    ${PROJECT_SOURCE_DIR}/texture_manager.cpp
    ${PROJECT_SOURCE_DIR}/texture_manager.hpp
    ${PROJECT_SOURCE_DIR}/vector_map.hpp

    PARENT_SCOPE)
