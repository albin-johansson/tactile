// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <cstddef>     // size_t
#include <cstdint>     // int32_t
#include <filesystem>  // path
#include <ostream>     // ostream
#include <string>      // string
#include <variant>     // variant
#include <vector>      // vector

#include "tactile/base/container/string_map.hpp"
#include "tactile/base/id.hpp"
#include "tactile/base/meta/attribute.hpp"
#include "tactile/base/numeric/index_2d.hpp"
#include "tactile/base/numeric/vec.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile::godot {

/** External resource identifier. */
using ExtResourceId = std::int32_t;

/** Subresource identifier. */
using SubResourceId = std::int32_t;

struct Gd3Metadata final
{
  StringMap<Attribute> props;
  StringMap<StringMap<Attribute>> comps;
};

struct Gd3ExtResource final
{
  std::string path;
  std::string type;
};

struct Gd3AtlasTexture final
{
  ExtResourceId atlas_id;
  Int4 region;
};

struct Gd3Animation final
{
  std::string name;
  std::vector<SubResourceId> frames;  // Atlas textures
  float speed;
};

struct Gd3SpriteFrames final
{
  SubResourceId id;
  std::vector<Gd3Animation> animations;
};

struct Gd3RectShape final
{
  Float2 extents;
};

struct Gd3Point final
{};

struct Gd3Rect final
{
  SubResourceId shape_id;
};

struct Gd3Polygon final
{
  std::vector<Float2> points;
};

struct Gd3Object final
{
  constexpr static std::size_t kPointTypeIndex = 0;
  constexpr static std::size_t kRectTypeIndex = 1;
  constexpr static std::size_t kPolygonTypeIndex = 2;

  std::string name;
  std::string parent;
  Float2 position;
  Gd3Metadata meta;
  std::variant<Gd3Point, Gd3Rect, Gd3Polygon> value;
  bool visible;
};

struct Gd3ObjectLayer final
{
  std::vector<Gd3Object> objects;
};

struct Gd3EncodedTile final
{
  std::int32_t position;
  std::int32_t tileset;
  std::int32_t tile_index;
};

struct Gd3TileAnimation final
{
  std::string parent;
  Index2D position;
  TileID tile_id;
};

struct Gd3Layer;

struct Gd3GroupLayer final
{
  std::vector<Gd3Layer> layers;
};

struct Gd3TileLayer final
{
  Int2 cell_size;
  std::vector<Gd3EncodedTile> tiles;
  std::vector<Gd3TileAnimation> animations;
};

struct Gd3Layer final
{
  constexpr static std::size_t kTileLayerTypeIndex = 0;
  constexpr static std::size_t kObjectLayerTypeIndex = 1;
  constexpr static std::size_t kGroupLayerTypeIndex = 2;

  std::string name;
  std::string parent;
  std::variant<Gd3TileLayer, Gd3ObjectLayer, Gd3GroupLayer> value;
  Gd3Metadata meta;
  float opacity;
  bool visible;
};

using Gd3ExtResourceMap = std::unordered_map<ExtResourceId, Gd3ExtResource>;
using Gd3AtlasTextureMap = std::unordered_map<SubResourceId, Gd3AtlasTexture>;
using Gd3RectShapeMap = std::unordered_map<SubResourceId, Gd3RectShape>;

struct Gd3Scene final  // TODO rename
{
  ExtResourceId next_ext_resource_id;
  SubResourceId next_sub_resource_id;
  Gd3ExtResourceMap ext_resources;
  Gd3Metadata root_meta;
};

struct Gd3TilesetInstance final
{
  std::string name;
  ExtResourceId texture_id;
  TileID first_tile_id;
  std::int32_t tile_count;
  std::int32_t column_count;
  Int2 tile_size;
  Int2 image_size;
};

struct Gd3Tileset final
{
  Gd3Scene scene;
  std::vector<std::filesystem::path> texture_paths;
  std::vector<Gd3TilesetInstance> instances;
};

struct Gd3Map final
{
  Gd3Scene scene;
  ExtResourceId tileset_id;
  Gd3Tileset tileset;
  std::vector<Gd3Layer> layers;
  std::unordered_map<TileID, ExtResourceId> tileset_texture_ids;
  Gd3SpriteFrames sprite_frames;
  Gd3AtlasTextureMap atlas_textures;
  Gd3RectShapeMap rect_shapes;
};

[[nodiscard]]
inline auto find_tileset_instance(const Gd3Tileset& gd_tileset, const TileID tile_id)
    -> const Gd3TilesetInstance*
{
  for (const auto& gd_tileset_instance : gd_tileset.instances) {
    const auto first_id = gd_tileset_instance.first_tile_id;
    const auto last_id = gd_tileset_instance.first_tile_id + gd_tileset_instance.tile_count;

    if (tile_id >= first_id && tile_id < last_id) {
      return &gd_tileset_instance;
    }
  }

  return nullptr;
}

[[nodiscard]]
inline auto get_tileset_index(const Gd3Tileset& gd_tileset, const TileID tile_id)
    -> std::int32_t
{
  std::int32_t tileset_index {};

  for (const auto& gd_tileset_instance : gd_tileset.instances) {
    const auto first_id = gd_tileset_instance.first_tile_id;
    const auto last_id = gd_tileset_instance.first_tile_id + gd_tileset_instance.tile_count;

    if (tile_id >= first_id && tile_id < last_id) {
      return tileset_index;
    }

    ++tileset_index;
  }

  return tileset_index;
}

}  // namespace tactile::godot
