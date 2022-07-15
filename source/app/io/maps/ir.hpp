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

#include <string>   // string
#include <variant>  // variant
#include <vector>   // vector

#include "core/attribute.hpp"
#include "core/common/associative.hpp"
#include "core/common/filesystem.hpp"
#include "core/common/identifiers.hpp"
#include "core/common/ints.hpp"
#include "core/common/macros.hpp"
#include "core/common/math.hpp"
#include "core/common/memory.hpp"
#include "core/layer/layer_type.hpp"
#include "core/layer/object_type.hpp"

namespace tactile::ir {

using ComponentMap = TreeMap<std::string, Attribute>;

struct ContextData final
{
  TreeMap<std::string, Attribute>    properties;
  TreeMap<std::string, ComponentMap> components;
};

struct ObjectData final
{
  ObjectID   id {};
  ObjectType type {};

  Vector2f pos {};
  Vector2f size {};

  std::string name;
  std::string tag;

  ContextData context;

  bool visible {};
};

struct TileLayerData final
{
  /* The sizes are provided for convenience, they should mirror the MapData values */
  usize                            row_count {};
  usize                            col_count {};
  std::vector<std::vector<TileID>> tiles;
};

struct ObjectLayerData final
{
  std::vector<ObjectData> objects;
};

struct LayerData;

struct GroupLayerData final
{
  GroupLayerData() = default;
  ~GroupLayerData() noexcept = default;

  TACTILE_DELETE_COPY(GroupLayerData);
  TACTILE_DEFAULT_MOVE(GroupLayerData);

  std::vector<Unique<LayerData>> children;
};

struct LayerData final
{
  using data_type = std::variant<TileLayerData, ObjectLayerData, GroupLayerData>;

  LayerID   id {};
  LayerType type {};

  usize index {};  /// Local index.

  std::string name;
  data_type   data;

  ContextData context;

  float opacity {};
  bool  visible {};
};

struct MetaAnimationFrameData final
{
  TileID local_id {};
  uint64 duration_ms {};
};

struct MetaTileData final
{
  std::vector<ObjectData>             objects;
  std::vector<MetaAnimationFrameData> frames;
  ContextData                         context;
};

struct TilesetData final
{
  std::string name;
  TileID      first_tile {};

  Vector2i tile_size {};

  int32 tile_count {};
  int32 column_count {};

  fs::path image_path;
  Vector2i image_size {};

  HashMap<TileIndex, MetaTileData> fancy_tiles;

  ContextData context;
};

struct MapData
{
  usize row_count {};
  usize col_count {};

  Vector2i tile_size {};

  int32 next_layer_id {};
  int32 next_object_id {};

  TreeMap<std::string, ComponentMap> component_definitions;

  std::vector<TilesetData> tilesets;
  std::vector<LayerData>   layers;

  ContextData context;
};

}  // namespace tactile::ir