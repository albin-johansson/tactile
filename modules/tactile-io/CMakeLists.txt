cmake_minimum_required(VERSION 3.18)

project(tactile-io CXX)

set(TACTILE_IO_SOURCES
    parse_error.cpp
    parse_error.hpp
    parse_ir.hpp

    yaml/yaml_parser.hpp

    yaml/internal/parse_fancy_tiles.hpp
    yaml/internal/parse_layers.hpp
    yaml/internal/parse_object_layer.hpp
    yaml/internal/parse_properties.hpp
    yaml/internal/parse_tile_layer.hpp
    yaml/internal/parse_tilesets.hpp

    yaml/internal/parse_fancy_tiles.cpp
    yaml/internal/parse_layers.cpp
    yaml/internal/parse_object_layer.cpp
    yaml/internal/parse_properties.cpp
    yaml/internal/parse_tile_layer.cpp
    yaml/internal/parse_tilesets.cpp
    yaml/internal/yaml_parser.cpp
    api.hpp)

add_library(TactileIO SHARED ${TACTILE_IO_SOURCES})

target_include_directories(TactileIO
    PRIVATE
    ${ROOT_DIR}/src
    ${ROOT_DIR}/src/tactile-io

    SYSTEM PRIVATE
    ${LIBRARY_DIR}/magic-enum
    ${LIBRARY_DIR}/tl
    ${LIBRARY_DIR}/nenya)

target_link_libraries(TactileIO
    PUBLIC
    yaml-cpp
    centurion)

target_compile_definitions(TactileIO PRIVATE
    TACTILE_IO_EXPORT
    CENTURION_NO_SDL_MIXER)