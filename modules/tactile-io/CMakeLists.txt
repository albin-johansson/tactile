cmake_minimum_required(VERSION 3.16)

project(tactile-io CXX)

set(TACTILE_IO_SOURCES
    ${PUGIXML_SOURCES}

    common_saving.cpp
    common_saving.hpp
    ir.cpp
    parse_error.cpp

    include/tactile-io/api.hpp
    include/tactile-io/emitter.hpp
    include/tactile-io/ir.hpp
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

add_library(TactileIO SHARED ${TACTILE_IO_SOURCES})

target_include_directories(TactileIO
    PRIVATE
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/include/tactile-io
    ${PROJECT_SOURCE_DIR}/../tactile-base/include

    SYSTEM PRIVATE
    ${LIBRARY_DIR}/entt
    ${LIBRARY_DIR}/nenya
    ${LIBRARY_DIR}/magic-enum
    ${LIBRARY_DIR}/json
    ${LIBRARY_DIR}/pugixml
    )

target_link_libraries(TactileIO
    PRIVATE
    yaml-cpp
    centurion
    fmt)

target_compile_definitions(TactileIO PRIVATE
    TACTILE_IO_EXPORT
    CENTURION_NO_SDL_MIXER)

if (MSVC)
  target_compile_options(TactileIO PRIVATE
      /EHsc
      /MP
      /W4
      /Zc:preprocessor
      /Zc:__cplusplus
      )
endif ()

target_precompile_headers(TactileIO PRIVATE
    <algorithm>
    <functional>
    <string>
    <vector>
    <filesystem>
    <utility>
    <memory>
    <optional>

    <centurion.hpp>
    <json.hpp>
    <magic_enum.hpp>
    <yaml-cpp/yaml.h>)