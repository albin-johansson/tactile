cmake_minimum_required(VERSION 3.16)

project(tactile-app-io-maps-emitter CXX)

set(TACTILE_IO_MAPS_EMITTER_SOURCE_FILES

    ${PROJECT_SOURCE_DIR}/emit_info.cpp
    ${PROJECT_SOURCE_DIR}/emit_info.hpp
    ${PROJECT_SOURCE_DIR}/json_emitter.cpp
    ${PROJECT_SOURCE_DIR}/json_emitter.hpp
    ${PROJECT_SOURCE_DIR}/yaml_emitter.cpp
    ${PROJECT_SOURCE_DIR}/yaml_emitter.hpp

    PARENT_SCOPE)
