cmake_minimum_required(VERSION 3.16)

project(tactile-io CXX)

find_package(yaml-cpp CONFIG REQUIRED)
find_package(pugixml CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(magic_enum CONFIG REQUIRED)

set(TACTILE_IO_SOURCES
    common_parsing.cpp
    common_parsing.hpp
    common_saving.cpp
    common_saving.hpp
    ir.cpp
    ir_definitions.hpp
    ir_map.cpp
    ir_object.cpp
    ir_property.cpp
    ir_tile.cpp
    ir_tileset.cpp
    parse_error.cpp
    string_utils.hpp

    include/tactile_io.hpp

    include/tactile-io/api.hpp
    include/tactile-io/emitter.hpp
    include/tactile-io/ir.hpp
    include/tactile-io/ir_common.hpp
    include/tactile-io/ir_helpers.hpp
    include/tactile-io/ir_map.hpp
    include/tactile-io/ir_object.hpp
    include/tactile-io/ir_property.hpp
    include/tactile-io/ir_tile.hpp
    include/tactile-io/ir_tileset.hpp
    include/tactile-io/parse_error.hpp
    include/tactile-io/parser.hpp

    json/json_common.hpp

    json/emitter/emit_json_map.cpp
    json/emitter/save_fancy_tiles.cpp
    json/emitter/save_fancy_tiles.hpp
    json/emitter/save_json.cpp
    json/emitter/save_json.hpp
    json/emitter/save_layers.cpp
    json/emitter/save_layers.hpp
    json/emitter/save_object.cpp
    json/emitter/save_object.hpp
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

    xml/emitter/append_fancy_tiles.cpp
    xml/emitter/append_fancy_tiles.hpp
    xml/emitter/append_layer.cpp
    xml/emitter/append_layer.hpp
    xml/emitter/append_object.cpp
    xml/emitter/append_object.hpp
    xml/emitter/append_properties.cpp
    xml/emitter/append_properties.hpp
    xml/emitter/append_tileset.cpp
    xml/emitter/append_tileset.hpp
    xml/emitter/emit_xml_map.cpp
    xml/emitter/save_xml.cpp
    xml/emitter/save_xml.hpp

    xml/parser/parse_csv.cpp
    xml/parser/parse_csv.hpp
    xml/parser/parse_fancy_tiles.cpp
    xml/parser/parse_fancy_tiles.hpp
    xml/parser/parse_layers.cpp
    xml/parser/parse_layers.hpp
    xml/parser/parse_object.cpp
    xml/parser/parse_object.hpp
    xml/parser/parse_object_layer.cpp
    xml/parser/parse_object_layer.hpp
    xml/parser/parse_properties.cpp
    xml/parser/parse_properties.hpp
    xml/parser/parse_tile_data.cpp
    xml/parser/parse_tile_data.hpp
    xml/parser/parse_tile_layer.cpp
    xml/parser/parse_tile_layer.hpp
    xml/parser/parse_tile_nodes.cpp
    xml/parser/parse_tile_nodes.hpp
    xml/parser/parse_tilesets.cpp
    xml/parser/parse_tilesets.hpp
    xml/parser/parse_xml_map.cpp
    xml/parser/xml_utils.cpp
    xml/parser/xml_utils.hpp

    yaml/yaml_fwd.hpp

    yaml/emitter/emit_yaml_map.cpp
    yaml/emitter/save_layers.cpp
    yaml/emitter/save_layers.hpp
    yaml/emitter/save_object.cpp
    yaml/emitter/save_object.hpp
    yaml/emitter/save_properties.cpp
    yaml/emitter/save_properties.hpp
    yaml/emitter/save_tilesets.cpp
    yaml/emitter/save_tilesets.hpp

    yaml/parser/parse_fancy_tiles.cpp
    yaml/parser/parse_fancy_tiles.hpp
    yaml/parser/parse_layers.cpp
    yaml/parser/parse_layers.hpp
    yaml/parser/parse_object.cpp
    yaml/parser/parse_object.hpp
    yaml/parser/parse_object_layer.cpp
    yaml/parser/parse_object_layer.hpp
    yaml/parser/parse_properties.cpp
    yaml/parser/parse_properties.hpp
    yaml/parser/parse_tile_layer.cpp
    yaml/parser/parse_tile_layer.hpp
    yaml/parser/parse_tilesets.hpp
    yaml/parser/parse_tilesets.cpp
    yaml/parser/parse_yaml_map.cpp
    )

add_library(${TACTILE_IO} SHARED ${TACTILE_IO_SOURCES})

target_include_directories(${TACTILE_IO}
    PRIVATE
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/include/tactile-io
    ${PROJECT_SOURCE_DIR}/../tactile-base

    SYSTEM PRIVATE
    ${LIBRARY_DIR}/nenya
    )

target_link_libraries(${TACTILE_IO}
    PRIVATE
    fmt::fmt
    magic_enum::magic_enum
    yaml-cpp
    nlohmann_json::nlohmann_json
    pugixml::pugixml
    )

target_compile_definitions(${TACTILE_IO}
    PRIVATE
    TACTILE_IO_EXPORT
    CENTURION_NO_SDL_MIXER
    )

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  target_compile_options(${TACTILE_IO}
      PRIVATE
      /EHsc
      /MP
      /W4
      /Zc:preprocessor
      /Zc:__cplusplus
      )
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_compile_options(${TACTILE_IO}
      PRIVATE
      -Wall
      -Wextra
      )
endif ()

target_precompile_headers(${TACTILE_IO} PRIVATE
    <cstdint>
    <cstddef>
    <cassert>
    <memory>
    <utility>
    <array>
    <vector>
    <map>
    <unordered_map>
    <variant>
    <string>
    <string_view>
    <concepts>
    <functional>

    <magic_enum.hpp>
    <yaml-cpp/yaml.h>
    <nlohmann/json.hpp>
    <pugixml.hpp>
    <fmt/format.h>
    )