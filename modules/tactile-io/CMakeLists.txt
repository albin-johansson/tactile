cmake_minimum_required(VERSION 3.18)

project(tactile-io CXX)

set(TACTILE_IO_SOURCES
    api.hpp
    parse_error.cpp
    parse_error.hpp
    parse_ir.hpp
    parser.hpp

    yaml/parse_fancy_tiles.cpp
    yaml/parse_fancy_tiles.hpp
    yaml/parse_layers.cpp
    yaml/parse_layers.hpp
    yaml/parse_object_layer.cpp
    yaml/parse_object_layer.hpp
    yaml/parse_properties.cpp
    yaml/parse_properties.hpp
    yaml/parse_tile_layer.cpp
    yaml/parse_tile_layer.hpp
    yaml/parse_tilesets.hpp
    yaml/parse_tilesets.cpp
    yaml/parse_yaml_map.cpp
    yaml/yaml_fwd.hpp
    )

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