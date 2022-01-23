cmake_minimum_required(VERSION 3.16)

project(tactile-app-core-algorithms CXX)

set(TACTILE_CORE_ALGORITHM_SOURCE_FILES

    ${PROJECT_SOURCE_DIR}/flood_fill.cpp
    ${PROJECT_SOURCE_DIR}/flood_fill.hpp
    ${PROJECT_SOURCE_DIR}/invoke_n.hpp

    PARENT_SCOPE)
