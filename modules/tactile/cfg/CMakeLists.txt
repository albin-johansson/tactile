cmake_minimum_required(VERSION 3.16)

project(tactile-app-cfg CXX)

set(TACTILE_CFG_SOURCE_FILES

    ${PROJECT_SOURCE_DIR}/configuration.cpp
    ${PROJECT_SOURCE_DIR}/configuration.hpp
    ${PROJECT_SOURCE_DIR}/imgui_context.cpp
    ${PROJECT_SOURCE_DIR}/imgui_context.hpp
    ${PROJECT_SOURCE_DIR}/protobuf_context.cpp
    ${PROJECT_SOURCE_DIR}/protobuf_context.hpp
    ${PROJECT_SOURCE_DIR}/sdl_attributes.cpp
    ${PROJECT_SOURCE_DIR}/sdl_attributes.hpp

    PARENT_SCOPE)
