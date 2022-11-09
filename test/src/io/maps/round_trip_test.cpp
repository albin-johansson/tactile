/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/**
 * This test suite performs "round trip" checks in order to make sure that no information
 * is lost or corrupted during emission or parsing.
 *
 * The source map data used is not accurate in order to avoid reused values in order to
 * make it easier to find mistakes in parser/emitter logic, such as mixed up tile width
 * and height, etc.
 */

#include <gtest/gtest.h>

#include "core/tile_matrix.hpp"
#include "core/type/string.hpp"
#include "core/util/assoc.hpp"
#include "io/map/emit/emit_info.hpp"
#include "io/map/emit/emitter.hpp"
#include "io/map/parse/parse_map.hpp"

using namespace std::string_literals;

namespace tactile::test {
namespace {

inline String current_parser;

void validate_contexts(const ir::ContextData& source, const ir::ContextData& restored)
{
  ASSERT_EQ(source.properties.size(), restored.properties.size());
  ASSERT_EQ(source.components.size(), restored.components.size());

  ASSERT_EQ(source.properties, restored.properties);

  for (const auto& [name, attributes]: source.components) {
    ASSERT_TRUE(has_key(restored.components, name));
    ASSERT_EQ(attributes, restored.components.at(name));
  }
}

void validate_objects(const ir::ObjectData& source, const ir::ObjectData& restored)
{
  ASSERT_EQ(source.id, restored.id);
  ASSERT_EQ(source.type, restored.type);

  ASSERT_EQ(source.pos, restored.pos);
  ASSERT_EQ(source.size, restored.size);

  ASSERT_EQ(source.name, restored.name);
  ASSERT_EQ(source.tag, restored.tag);

  ASSERT_EQ(source.visible, restored.visible);

  validate_contexts(source.context, restored.context);
}

void validate_object_layers(const ir::ObjectLayerData& source,
                            const ir::ObjectLayerData& restored)
{
  ASSERT_EQ(source.objects.size(), restored.objects.size());
  for (usize index = 0; index < source.objects.size(); ++index) {
    const auto& source_object = source.objects.at(index);
    const auto& restored_object = restored.objects.at(index);
    validate_objects(source_object, restored_object);
  }
}

void validate_layers(const ir::LayerData& source_layer,
                     const ir::LayerData& restored_layer)
{
  ASSERT_EQ(source_layer.name, restored_layer.name);
  ASSERT_EQ(source_layer.type, restored_layer.type);

  ASSERT_EQ(source_layer.id, restored_layer.id);
  ASSERT_EQ(source_layer.index, restored_layer.index);

  ASSERT_EQ(source_layer.opacity, restored_layer.opacity);
  ASSERT_EQ(source_layer.visible, restored_layer.visible);

  if (source_layer.type == LayerType::TileLayer) {
    const auto& source_tile_data = source_layer.as_tile_layer();
    const auto& restored_tile_data = restored_layer.as_tile_layer();

    ASSERT_EQ(source_tile_data.row_count, restored_tile_data.row_count);
    ASSERT_EQ(source_tile_data.col_count, restored_tile_data.col_count);
    ASSERT_EQ(source_tile_data.tiles, restored_tile_data.tiles);
  }
  else if (source_layer.type == LayerType::ObjectLayer) {
    const auto& source_objects = source_layer.as_object_layer();
    const auto& restored_objects = restored_layer.as_object_layer();
    validate_object_layers(source_objects, restored_objects);
  }
  else if (source_layer.type == LayerType::GroupLayer) {
    const auto& source_group = source_layer.as_group_layer();
    const auto& restored_group = restored_layer.as_group_layer();

    ASSERT_EQ(source_group.children.size(), restored_group.children.size());
    for (usize index = 0; index < source_group.children.size(); ++index) {
      const auto& source_child_layer = source_group.children.at(index);
      const auto& restored_child_layer = restored_group.children.at(index);
      validate_layers(*source_child_layer, *restored_child_layer);
    }
  }

  validate_contexts(source_layer.context, restored_layer.context);
}

void validate_layers(const ir::MapData& source, const ir::MapData& restored)
{
  ASSERT_EQ(source.layers.size(), restored.layers.size());
  for (usize index = 0; index < source.layers.size(); ++index) {
    const auto& source_layer = source.layers.at(index);
    const auto& restored_layer = restored.layers.at(index);
    validate_layers(source_layer, restored_layer);
  }
}

void validate_fancy_tiles(const ir::MetaTileData& source,
                          const ir::MetaTileData& restored)
{
  ASSERT_EQ(source.objects.size(), restored.objects.size());
  for (usize index = 0; index < source.objects.size(); ++index) {
    const auto& source_object = source.objects.at(index);
    const auto& restored_object = restored.objects.at(index);
    validate_objects(source_object, restored_object);
  }

  ASSERT_EQ(source.frames.size(), restored.frames.size());
  for (usize index = 0; index < source.frames.size(); ++index) {
    const auto& source_frame = source.frames.at(index);
    const auto& restored_frame = restored.frames.at(index);

    ASSERT_EQ(source_frame.tile_index, restored_frame.tile_index);
    ASSERT_EQ(source_frame.duration_ms, restored_frame.duration_ms);
  }

  validate_contexts(source.context, restored.context);
}

void validate_tilesets(const ir::MapData& source, const ir::MapData& restored)
{
  ASSERT_EQ(source.tilesets.size(), restored.tilesets.size());

  for (usize index = 0; index < source.tilesets.size(); ++index) {
    const auto& source_tileset = source.tilesets.at(index);
    const auto& restored_tileset = restored.tilesets.at(index);

    ASSERT_EQ(source_tileset.name, restored_tileset.name);
    ASSERT_EQ(source_tileset.first_tile, restored_tileset.first_tile);

    ASSERT_EQ(source_tileset.tile_count, restored_tileset.tile_count);
    ASSERT_EQ(source_tileset.column_count, restored_tileset.column_count);

    ASSERT_EQ(source_tileset.tile_size, restored_tileset.tile_size);

    // ASSERT_EQ(sourceTileset.image_path, restoredTileset.image_path);
    ASSERT_EQ(source_tileset.image_size, restored_tileset.image_size);

    ASSERT_EQ(source_tileset.fancy_tiles.size(), restored_tileset.fancy_tiles.size());
    for (const auto& [id, sourceTile]: source_tileset.fancy_tiles) {
      const auto& restored_tile = restored_tileset.fancy_tiles.at(id);
      validate_fancy_tiles(sourceTile, restored_tile);
    }

    validate_contexts(source_tileset.context, restored_tileset.context);
  }
}

void validate_component_definitions(const ir::MapData& source,
                                    const ir::MapData& restored)
{
  ASSERT_EQ(source.component_definitions.size(), restored.component_definitions.size());
  ASSERT_EQ(source.component_definitions, restored.component_definitions);
}

void validate_basic_map_info(const ir::MapData& source, const ir::MapData& restored)
{
  ASSERT_EQ(source.row_count, restored.row_count);
  ASSERT_EQ(source.col_count, restored.col_count);

  ASSERT_EQ(source.tile_size, restored.tile_size);

  ASSERT_EQ(source.next_object_id, restored.next_object_id);
  ASSERT_EQ(source.next_layer_id, restored.next_layer_id);

  if (current_parser == "YAML") {
    ASSERT_EQ(source.tile_format.encoding, restored.tile_format.encoding);
    ASSERT_EQ(source.tile_format.compression, restored.tile_format.compression);
  }
}

constexpr usize row_count = 15;
constexpr usize col_count = 13;

[[nodiscard]] auto create_source_ground_layer() -> ir::LayerData
{
  ir::LayerData data;

  data.name = "Ground";
  data.type = LayerType::TileLayer;

  data.id = 1;
  data.index = 0;

  data.opacity = 0.8f;
  data.visible = true;

  auto& tile_data = data.data.emplace<ir::TileLayerData>();
  tile_data.row_count = row_count;
  tile_data.col_count = col_count;

  tile_data.tiles = make_tile_matrix(row_count, col_count);
  for (usize row = 0; row < row_count; ++row) {
    for (usize col = 0; col < col_count; ++col) {
      tile_data.tiles[row][col] = 7;
    }
  }

  data.context.properties["is-ground"] = true;
  data.context.properties["ratio"] = 0.5f;

  return data;
}

[[nodiscard]] auto create_source_group_layer(const bool use_components) -> ir::LayerData
{
  ir::LayerData data;

  data.name = "Tile Detail Layers";
  data.type = LayerType::GroupLayer;

  data.id = 2;
  data.index = 1;

  data.opacity = 1.0f;
  data.visible = false;

  data.context.properties["tint"] = cen::color::from_rgba("#1DBC748F").value();

  auto& group = data.data.emplace<ir::GroupLayerData>();

  {
    auto& child = group.children.emplace_back(std::make_unique<ir::LayerData>());
    child->name = "Details 1";
    child->type = LayerType::TileLayer;

    child->id = 3;
    child->index = 0;

    child->opacity = 0.9f;
    child->visible = true;

    auto& tile_data = child->data.emplace<ir::TileLayerData>();
    tile_data.row_count = row_count;
    tile_data.col_count = col_count;
    tile_data.tiles = make_tile_matrix(row_count, col_count);

    child->context.properties["path"] = fs::path {"resources/exterior.png"};
  }

  {
    auto& child = group.children.emplace_back(std::make_unique<ir::LayerData>());
    child->name = "Details 2";
    child->type = LayerType::TileLayer;

    child->id = 4;
    child->index = 1;

    child->opacity = 1.0f;
    child->visible = true;

    auto& tile_data = child->data.emplace<ir::TileLayerData>();
    tile_data.row_count = row_count;
    tile_data.col_count = col_count;
    tile_data.tiles = make_tile_matrix(row_count, col_count);

    if (use_components) {
      child->context.components["short-component"]["integer"] = 356;
    }
  }

  return data;
}

[[nodiscard]] auto create_source_object_layer(const bool use_components) -> ir::LayerData
{
  ir::LayerData data;

  data.name = "Objects";
  data.type = LayerType::ObjectLayer;

  data.id = 5;
  data.index = 2;

  data.opacity = 1.0f;
  data.visible = true;

  auto& object_layer_data = data.data.emplace<ir::ObjectLayerData>();

  auto& point = object_layer_data.objects.emplace_back();
  point.name = "Point";
  point.type = ObjectType::Point;
  point.id = 34;
  point.pos.x = 453;
  point.pos.y = 328;
  point.tag = "point-tag";
  point.visible = true;

  if (use_components) {
    point.context.components["empty-component"];
  }

  auto& rect = object_layer_data.objects.emplace_back();
  rect.name = "Rectangle";
  rect.type = ObjectType::Rect;
  rect.id = 26;
  rect.pos.x = 854;
  rect.pos.y = 654;
  rect.size.x = 123;
  rect.size.y = 68;
  rect.tag = "rect-tag";
  rect.visible = true;

  rect.context.properties["rect-int"] = 3'347;
  rect.context.properties["rect-color"] = cen::colors::hot_pink;

  if (use_components) {
    rect.context.components["short-component"]["integer"] = 83;
  }

  auto& ellipse = object_layer_data.objects.emplace_back();
  ellipse.name = "Ellipse";
  ellipse.type = ObjectType::Ellipse;
  ellipse.id = 54;
  ellipse.pos.x = 193;
  ellipse.pos.y = 587;
  ellipse.size.x = 34;
  ellipse.size.y = 39;
  ellipse.visible = true;

  return data;
}

[[nodiscard]] auto create_source_tileset(const bool use_components) -> ir::TilesetData
{
  ir::TilesetData data;

  data.name = "test_tileset";

  data.first_tile = 1;

  data.tile_size.x = 33;
  data.tile_size.y = 32;

  data.tile_count = 768;
  data.column_count = 28;

  data.image_path = "resources/terrain.png";
  data.image_size.x = 1'024;
  data.image_size.y = 1'023;

  data.fancy_tiles[155].frames = {{.tile_index = 155, .duration_ms = 150},
                                  {.tile_index = 156, .duration_ms = 120},
                                  {.tile_index = 157, .duration_ms = 145}};

  auto& object_data = data.fancy_tiles[80].objects.emplace_back();
  object_data.name = "Fancy Object";
  object_data.type = ObjectType::Point;
  object_data.id = 99;
  object_data.pos.x = 23;
  object_data.pos.y = 47;
  object_data.visible = false;

  data.fancy_tiles[27].context.properties["tile-float"] = 45.3f;

  if (use_components) {
    data.fancy_tiles[98].context.components["short-component"]["integer"] = 2'353;
  }

  data.context.properties["is-tileset"] = true;
  data.context.properties["user-id"] = 74;

  return data;
}

[[nodiscard]] auto create_source_data(const bool use_components) -> ir::MapData
{
  ir::MapData data;

  data.tile_size.x = 32;
  data.tile_size.y = 28;

  data.next_layer_id = 5;
  data.next_object_id = 8;

  data.row_count = row_count;
  data.col_count = col_count;

  data.tile_format.encoding = TileEncoding::Base64;
  data.tile_format.compression = TileCompression::Zlib;
  data.tile_format.zlib_compression_level = 6;

  if (use_components) {
    data.component_definitions["empty-component"];

    data.component_definitions["short-component"]["integer"] = 42;

    data.component_definitions["long-component"]["integer"] = 0;
    data.component_definitions["long-component"]["integer-v"] = 584;

    data.component_definitions["long-component"]["floating"] = 0.0f;
    data.component_definitions["long-component"]["floating-v"] = 83.4f;

    data.component_definitions["long-component"]["boolean"] = false;
    data.component_definitions["long-component"]["boolean-v"] = true;

    data.component_definitions["long-component"]["str"] = ""s;
    data.component_definitions["long-component"]["str-v"] = "hello"s;

    data.component_definitions["long-component"]["path"] = fs::path {};
    data.component_definitions["long-component"]["path-v"] = fs::path {"../foo.txt"};

    data.component_definitions["long-component"]["col"] = cen::colors::black;
    data.component_definitions["long-component"]["col-v"] = cen::colors::indigo;
  }

  data.tilesets.push_back(create_source_tileset(use_components));

  data.layers.push_back(create_source_ground_layer());
  data.layers.push_back(create_source_group_layer(use_components));
  data.layers.push_back(create_source_object_layer(use_components));

  data.context.properties["map-int"] = 123;
  data.context.properties["map-float"] = 56.3f;
  data.context.properties["map-bool"] = false;
  data.context.properties["map-string"] = "foobar"s;
  data.context.properties["map-file"] = fs::path {"foo/bar.txt"};
  data.context.properties["map-color"] = cen::colors::sea_green;
  data.context.properties["map-object"] = object_t {42};

  if (use_components) {
    data.context.components["empty-component"];
    data.context.components["long-component"] =
        data.component_definitions.at("long-component");
  }

  return data;
}

}  // namespace

TEST(RoundTrip, YAML)
{
  current_parser = "YAML";

  const io::EmitInfo emitter {fs::absolute("test_map.yaml"), create_source_data(true)};
  io::emit_yaml_map(emitter);

  const auto result = io::parse_map("test_map.yaml");
  ASSERT_EQ(io::ParseError::None, result.error());

  const auto& source = emitter.data();
  const auto& restored = result.data();

  validate_basic_map_info(source, restored);
  validate_component_definitions(source, restored);
  validate_tilesets(source, restored);
  validate_layers(source, restored);
}

TEST(RoundTrip, JSON)
{
  current_parser = "JSON";

  const io::EmitInfo emitter {fs::absolute("test_map.json"), create_source_data(false)};
  io::emit_json_map(emitter);

  const auto result = io::parse_map("test_map.json");
  ASSERT_EQ(io::ParseError::None, result.error());

  ASSERT_EQ(io::ParseError::None, result.error());

  const auto& source = emitter.data();
  const auto& restored = result.data();

  validate_basic_map_info(source, restored);
  validate_tilesets(source, restored);
  validate_layers(source, restored);
}

TEST(RoundTrip, XML)
{
  current_parser = "XML";

  const io::EmitInfo emitter {fs::absolute("test_map.tmx"), create_source_data(false)};
  io::emit_xml_map(emitter);

  const auto result = io::parse_map("test_map.tmx");
  ASSERT_EQ(io::ParseError::None, result.error());

  const auto& source = emitter.data();
  const auto& restored = result.data();

  validate_basic_map_info(source, restored);
  validate_tilesets(source, restored);
  validate_layers(source, restored);
}

}  // namespace tactile::test
