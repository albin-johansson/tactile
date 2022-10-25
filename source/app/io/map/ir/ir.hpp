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

#include <concepts>  // invocable
#include <variant>   // get

#include "core/attribute.hpp"
#include "core/layer/layer_type.hpp"
#include "core/layer/object_type.hpp"
#include "core/layer/tile_format.hpp"
#include "core/type/hash_map.hpp"
#include "core/type/math.hpp"
#include "core/type/maybe.hpp"
#include "core/type/path.hpp"
#include "core/type/ptr.hpp"
#include "core/type/string.hpp"
#include "core/type/tree_map.hpp"
#include "core/type/variant.hpp"
#include "core/type/vec.hpp"
#include "core/util/algorithm.hpp"
#include "core/uuid.hpp"
#include "core/vocabulary.hpp"

namespace tactile::ir {

using AttributeMap = TreeMap<String, Attribute>;
using ComponentMap = TreeMap<String, AttributeMap>;

struct ContextData final {
  AttributeMap properties;
  ComponentMap components;
};

struct ObjectData final {
  ObjectID id {};
  ObjectType type {};
  Float2 pos {};
  Float2 size {};
  String name;
  String tag;
  ContextData context;
  bool visible {};
};

struct TileLayerData final {
  // The sizes are provided for convenience, they should mirror the map dimensions.
  usize row_count {};
  usize col_count {};
  TileMatrix tiles;
};

struct ObjectLayerData final {
  Vec<ObjectData> objects;
};

struct LayerData;

struct GroupLayerData final {
  GroupLayerData() = default;
  ~GroupLayerData() noexcept = default;

  TACTILE_DELETE_COPY(GroupLayerData);
  TACTILE_DEFAULT_MOVE(GroupLayerData);

  Vec<Unique<LayerData>> children;
};

struct LayerData final {
  using Data = Variant<TileLayerData, ObjectLayerData, GroupLayerData>;

  LayerID id {};
  LayerType type {};
  usize index {};  /// Local index.
  String name;
  Data data;
  ContextData context;
  float opacity {};
  bool visible {};

  [[nodiscard]] auto as_tile_layer() const -> const TileLayerData&
  {
    return std::get<TileLayerData>(data);
  }

  [[nodiscard]] auto as_object_layer() const -> const ObjectLayerData&
  {
    return std::get<ObjectLayerData>(data);
  }

  [[nodiscard]] auto as_group_layer() const -> const GroupLayerData&
  {
    return std::get<GroupLayerData>(data);
  }
};

struct AnimationFrameData final {
  TileIndex tile_index {};
  uint64 duration_ms {};
};

struct MetaTileData final {
  UUID uuid {make_uuid()};  // This is not persistent! Only here for convenience.

  Vec<ObjectData> objects;
  Vec<AnimationFrameData> frames;
  ContextData context;
};

struct TilesetData final {
  using MetaTiles = HashMap<TileIndex, MetaTileData>;

  UUID uuid {make_uuid()};  // This is not persistent! Only here for convenience.

  String name;
  TileID first_tile {};
  Int2 tile_size {};
  int32 tile_count {};
  int32 column_count {};
  Path image_path;
  Int2 image_size {};
  MetaTiles fancy_tiles;
  ContextData context;

  [[nodiscard]] auto is_animated(const TileIndex index) const -> bool
  {
    const auto iter = fancy_tiles.find(index);
    return iter != fancy_tiles.end() && !iter->second.frames.empty();
  }
};

struct TileFormatData final {
  TileEncoding encoding {TileEncoding::Plain};
  TileCompression compression {TileCompression::None};
  Maybe<int32> zlib_compression_level;
  Maybe<int32> zstd_compression_level;
};

struct MapData final {
  usize row_count {};
  usize col_count {};
  Int2 tile_size {};
  int32 next_layer_id {};
  int32 next_object_id {};
  TileFormatData tile_format;
  ComponentMap component_definitions;
  Vec<TilesetData> tilesets;
  Vec<LayerData> layers;
  ContextData context;

  [[nodiscard]] auto find_tileset_with_tile(const TileID id) const -> const TilesetData&
  {
    return first_in(tilesets, [id](const ir::TilesetData& ts) {
      return id >= ts.first_tile && id <= ts.first_tile + ts.tile_count;
    });
  }
};

template <std::invocable<const ObjectLayerData&> T>
void each_object_layer(const GroupLayerData& root, T&& callable)
{
  for (const auto& layer: root.children) {
    if (layer->type == LayerType::GroupLayer) {
      const auto& group = layer->as_group_layer();
      each_object_layer(group, callable);
    }
    else if (layer->type == LayerType::ObjectLayer) {
      const auto& object_layer = layer->as_object_layer();
      callable(object_layer);
    }
  }
}

template <std::invocable<const ObjectLayerData&> T>
void each_object_layer(const MapData& map, T&& callable)
{
  for (const auto& layer: map.layers) {
    if (layer.type == LayerType::GroupLayer) {
      const auto& group = layer.as_group_layer();
      each_object_layer(group, callable);
    }
    else if (layer.type == LayerType::ObjectLayer) {
      const auto& object_layer = layer.as_object_layer();
      callable(object_layer);
    }
  }
}

}  // namespace tactile::ir