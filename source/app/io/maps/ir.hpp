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

#pragma once

#include <filesystem>  // path
#include <memory>      // unique_ptr
#include <string>      // string
#include <variant>     // variant
#include <vector>      // vector

#include "core/attribute.hpp"
#include "core/layer_type.hpp"
#include "core/object_type.hpp"
#include "tactile.hpp"

namespace tactile::ir {

using component_map = tree_map<std::string, attribute_value>;

struct attribute_context_data final
{
  tree_map<std::string, attribute_value> properties;
  tree_map<std::string, component_map> components;
};

struct object_data final
{
  object_id id{};
  object_type type{};

  float x{};
  float y{};
  float width{};
  float height{};

  std::string name;
  std::string tag;

  attribute_context_data context;

  bool visible{};
};

struct tile_layer_data final
{
  /* The sizes are provided for convenience, they should mirror the map_data values */
  usize row_count{};
  usize col_count{};
  std::vector<std::vector<tile_id>> tiles;
};

struct object_layer_data final
{
  std::vector<object_data> objects;
};

struct layer_data;

struct group_layer_data final
{
  group_layer_data() = default;
  ~group_layer_data() noexcept = default;

  TACTILE_DELETE_COPY(group_layer_data);
  TACTILE_DEFAULT_MOVE(group_layer_data);

  std::vector<std::unique_ptr<layer_data>> children;
};

struct layer_data final
{
  using data_type = std::variant<tile_layer_data, object_layer_data, group_layer_data>;

  layer_id id{};
  layer_type type{};

  usize index{};

  std::string name;
  data_type data;

  attribute_context_data context;

  float opacity{};
  bool visible{};
};

struct fancy_animation_frame_data final
{
  tile_id local_id{};
  uint64 duration_ms{};
};

struct fancy_tile_data final
{
  std::vector<object_data> objects;
  std::vector<fancy_animation_frame_data> frames;
  attribute_context_data context;
};

struct tileset_data final
{
  std::string name;
  tile_id first_tile{};

  int32 tile_width{};
  int32 tile_height{};

  int32 tile_count{};
  int32 column_count{};

  std::filesystem::path image_path;
  int32 image_width{};
  int32 image_height{};

  hash_map<tile_id, fancy_tile_data> fancy_tiles;

  attribute_context_data context;
};

struct map_data
{
  usize row_count{};
  usize col_count{};

  int32 tile_width{};
  int32 tile_height{};

  int32 next_layer_id{};
  int32 next_object_id{};

  tree_map<std::string, component_map> component_definitions;

  std::vector<tileset_data> tilesets;
  std::vector<layer_data> layers;

  attribute_context_data context;
};

}  // namespace tactile::ir