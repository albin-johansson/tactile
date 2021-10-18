cmake_minimum_required(VERSION 3.18)

project(tactile-io CXX)

set(TACTILE_IO_SOURCES
    api.hpp
    common_saving.cpp
    common_saving.hpp
    emitter.hpp
    parse_error.cpp
    parse_error.hpp
    parse_ir.hpp
    parser.hpp

    json/json_common.hpp

    json/emitter/save_fancy_tiles.cpp
    json/emitter/save_fancy_tiles.hpp
    json/emitter/save_json.cpp
    json/emitter/save_json.hpp
    json/emitter/save_json_map.cpp
    json/emitter/save_layers.cpp
    json/emitter/save_layers.hpp
    json/emitter/save_properties.cpp
    json/emitter/save_properties.hpp
    json/emitter/save_tilesets.cpp
    json/emitter/save_tilesets.hpp

    json/parser/parse_fancy_tiles.cpp
    json/parser/parse_fancy_tiles.hpp
    json/parser/parse_json_map.cpp
    json/parser/parse_layers.cpp
    json/parser/parse_layers.hpp
    json/parser/parse_object_layer.cpp
    json/parser/parse_object_layer.hpp
    json/parser/parse_properties.cpp
    json/parser/parse_properties.hpp
    json/parser/parse_tile_data.cpp
    json/parser/parse_tile_data.hpp
    json/parser/parse_tile_layer.cpp
    json/parser/parse_tile_layer.hpp
    json/parser/parse_tileset.cpp
    json/parser/parse_tileset.hpp
    json/parser/parse_tilesets.cpp
    json/parser/parse_tilesets.hpp
    json/parser/read_json.cpp
    json/parser/read_json.hpp

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
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/..

    SYSTEM PRIVATE
    ${LIBRARY_DIR}/entt
    ${LIBRARY_DIR}/nenya
    ${LIBRARY_DIR}/tl
    ${LIBRARY_DIR}/magic-enum
    ${LIBRARY_DIR}/json
    )

target_link_libraries(TactileIO
    PUBLIC
    yaml-cpp
    centurion)

target_compile_definitions(TactileIO PRIVATE
    TACTILE_IO_EXPORT
    CENTURION_NO_SDL_MIXER)

target_precompile_headers(TactileIO PRIVATE
    <centurion.hpp>
    <expected.hpp>
    <json.hpp>
    <magic_enum.hpp>
    <yaml-cpp/yaml.h>)