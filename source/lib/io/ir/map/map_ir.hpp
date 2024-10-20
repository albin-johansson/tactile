// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <concepts>  // invocable
#include <memory>    // addressof
#include <variant>   // get

#include "common/util/algorithm.hpp"
#include "core/layer/layer_type.hpp"
#include "core/layer/object_type.hpp"
#include "core/layer/tile_format.hpp"
#include "core/tile/tile_extent.hpp"
#include "core/tile/tile_matrix.hpp"
#include "tactile/base/container/hash_map.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/path.hpp"
#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/base/container/string.hpp"
#include "tactile/base/container/string_map.hpp"
#include "tactile/base/container/variant.hpp"
#include "tactile/base/container/vector.hpp"
#include "tactile/base/int.hpp"
#include "tactile/base/meta/attribute.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/numeric/vec.hpp"
#include "tactile/core/util/uuid.hpp"

namespace tactile {

TACTILE_FWD_DECLARE_STRUCT(LayerIR)

using AttributeMap = StringMap<Attribute>;
using ComponentMap = StringMap<AttributeMap>;

struct ContextIR final
{
  AttributeMap properties;
  ComponentMap components;
};

struct ObjectIR final
{
  ObjectID id {};
  ObjectType type {};
  Float2 pos {};
  Float2 size {};
  String name;
  String tag;
  ContextIR context;
  bool visible {};
};

struct TileLayerIR final
{
  // The extent is provided for convenience, it should mirror the map extent.
  TileExtent extent;
  TileMatrix tiles;
};

struct ObjectLayerIR final
{
  Vector<ObjectIR> objects;
};

struct GroupLayerIR final
{
  GroupLayerIR() = default;
  ~GroupLayerIR() noexcept = default;

  TACTILE_DELETE_COPY(GroupLayerIR);
  TACTILE_DEFAULT_MOVE(GroupLayerIR);

  Vector<Unique<LayerIR>> children;
};

struct LayerIR final
{
  using Data = Variant<TileLayerIR, ObjectLayerIR, GroupLayerIR>;

  LayerID id {};
  LayerType type {};
  usize index {};  /// Local index.
  String name;
  Data data;
  ContextIR context;
  float opacity {};
  bool visible {};

  [[nodiscard]] auto as_tile_layer() const -> const TileLayerIR&
  {
    return std::get<TileLayerIR>(data);
  }

  [[nodiscard]] auto as_object_layer() const -> const ObjectLayerIR&
  {
    return std::get<ObjectLayerIR>(data);
  }

  [[nodiscard]] auto as_group_layer() const -> const GroupLayerIR&
  {
    return std::get<GroupLayerIR>(data);
  }
};

struct AnimationFrameIR final
{
  TileIndex tile_index {};
  uint64 duration_ms {};
};

struct TileIR final
{
  UUID uuid {UUID::generate()};  // This is not persistent! Only here for convenience.

  Vector<ObjectIR> objects;
  Vector<AnimationFrameIR> frames;
  ContextIR context;
};

struct TilesetIR final
{
  using MetaTiles = HashMap<TileIndex, TileIR>;

  UUID uuid {UUID::generate()};  // This is not persistent! Only here for convenience.

  String name;
  TileID first_tile {};
  Int2 tile_size {};
  int32 tile_count {};
  int32 column_count {};
  Path image_path;
  Int2 image_size {};
  MetaTiles fancy_tiles;
  ContextIR context;

  [[nodiscard]] auto is_animated(const TileIndex index) const -> bool
  {
    const auto iter = fancy_tiles.find(index);
    return iter != fancy_tiles.end() && !iter->second.frames.empty();
  }
};

struct TileFormatIR final
{
  TileEncoding encoding {TileEncoding::kPlainText};
  CompressionType compression {CompressionType::kNone};
  Maybe<int32> zlib_compression_level;
  Maybe<int32> zstd_compression_level;
};

struct MapIR final
{
  TileExtent extent;
  Int2 tile_size {};
  int32 next_layer_id {};
  int32 next_object_id {};
  TileFormatIR tile_format;
  ComponentMap component_definitions;
  Vector<TilesetIR> tilesets;
  Vector<LayerIR> layers;
  ContextIR context;

  [[nodiscard]] auto find_tileset_with_tile(const TileID id) const -> const TilesetIR&
  {
    return first_in(tilesets, [id](const TilesetIR& ts) {
      return id >= ts.first_tile && id <= ts.first_tile + ts.tile_count;
    });
  }
};

template <typename T, std::invocable<const ObjectLayerIR&> U>
void each_object_layer(const T& root, U&& callable)
{
  for (const auto& layer : root.children) {
    const auto* layer_ptr = std::addressof(layer);
    if (layer_ptr->type == LayerType::GroupLayer) {
      const auto& group_layer = layer_ptr->as_group_layer();
      each_object_layer(group_layer, callable);
    }
    else if (layer_ptr->type == LayerType::ObjectLayer) {
      const auto& object_layer = layer_ptr->as_object_layer();
      callable(object_layer);
    }
  }
}

}  // namespace tactile
