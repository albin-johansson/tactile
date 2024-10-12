// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <chrono>      // milliseconds
#include <filesystem>  // path
#include <optional>    // optional
#include <string>      // string
#include <vector>      // vector

#include "tactile/base/id.hpp"
#include "tactile/base/io/compress/compression_format_id.hpp"
#include "tactile/base/layer/layer_type.hpp"
#include "tactile/base/layer/object_type.hpp"
#include "tactile/base/layer/tile_encoding.hpp"
#include "tactile/base/meta/attribute.hpp"
#include "tactile/base/numeric/extent_2d.hpp"
#include "tactile/base/numeric/vec.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile::ir {

/**
 * Intermediate representation of an attribute with a given name.
 */
struct NamedAttribute final
{
  /** The attribute name. */
  std::string name;

  /** The attribute value. */
  Attribute value;

  [[nodiscard]]
  auto operator==(const NamedAttribute&) const -> bool = default;
};

/**
 * Intermediate representation of a component definition.
 */
struct Component final
{
  /** The component name. */
  std::string name;

  /** The component attribute defaults. */
  std::vector<NamedAttribute> attributes;

  [[nodiscard]]
  auto operator==(const Component&) const -> bool = default;
};

/**
 * Intermediate representation of a component instance, attached to a meta
 * context.
 */
struct AttachedComponent final
{
  /** The component type name. */
  std::string type;

  /** The component attributes. */
  std::vector<NamedAttribute> attributes;

  [[nodiscard]]
  auto operator==(const AttachedComponent&) const -> bool = default;
};

/**
 * Provides the data featured by all meta contexts.
 */
struct Metadata final
{
  /** The name of the context. */
  std::string name;

  /** The attached properties. */
  std::vector<NamedAttribute> properties;

  /** The attached components. */
  std::vector<AttachedComponent> components;

  [[nodiscard]]
  auto operator==(const Metadata&) const -> bool = default;
};

/**
 * Intermediate representation of a layer object.
 */
struct Object final
{
  /** Metadata for the object. */
  Metadata meta;

  /** The associated identifier. */
  ObjectID id;

  /** The type of the object. */
  ObjectType type;

  /** The position of the object. */
  Float2 position;

  /** The size of the object. */
  Float2 size;

  /** A user-provided label. */
  std::string tag;

  /** Whether the object is rendered. */
  bool visible;

  [[nodiscard]]
  auto operator==(const Object&) const -> bool = default;
};

/**
 * Intermediate representation of a layer.
 */
struct Layer final
{
  /** Metadata for the layer. */
  Metadata meta;

  /** The associated identifier. */
  ObjectID id;

  /** The type of the layer. */
  LayerType type;

  /** The opacity of the layer content. */
  float opacity;

  /** The number of tile rows and columns (if tile layer). */
  Extent2D extent;

  /** The contained tiles (if tile layer). */
  std::vector<std::vector<TileID>> tiles;

  /** The contained objects (if object layer). */
  std::vector<Object> objects;

  /** The contained layers (if group layer). */
  std::vector<Layer> layers;

  /** Whether the layer is rendered. */
  bool visible {};

  [[nodiscard]]
  auto operator==(const Layer&) const -> bool = default;
};

/**
 * Intermediate representation of a frame in a tile animation.
 */
struct AnimationFrame final
{
  /** The index of the rendered tile in the associated tileset. */
  TileIndex tile_index;

  /** The duration that the frame is shown. */
  std::chrono::milliseconds duration;

  [[nodiscard]]
  auto operator==(const AnimationFrame&) const -> bool = default;
};

using TileAnimation = std::vector<AnimationFrame>;

/**
 * Intermediate representation of a tile definition.
 */
struct Tile final
{
  /** Metadata for the tile. */
  Metadata meta;

  /** The index in the associated tileset. */
  TileIndex index;

  /** The contained objects, if any. */
  std::vector<Object> objects;

  /** The associated animation frames, if any. */
  TileAnimation animation;

  [[nodiscard]]
  auto operator==(const Tile&) const -> bool = default;
};

/**
 * Intermediate representation of a tileset definition.
 */
struct Tileset final
{
  /** Metadata for the tileset. */
  Metadata meta;

  /** The size of tiles in the tileset. */
  Int2 tile_size;

  /** The total number of tiles in the tileset. */
  std::ptrdiff_t tile_count;

  /** The number of tile columns in the tileset. */
  std::ptrdiff_t column_count;

  /** The size of the associated image. */
  Int2 image_size;

  /** The file path to the associated image. */
  std::filesystem::path image_path;

  /** The associated tile descriptors. */
  std::vector<Tile> tiles;

  /** Indicates whether the tileset is embedded. */
  bool is_embedded;

  [[nodiscard]]
  auto operator==(const Tileset&) const -> bool = default;
};

/**
 * Intermediate representation of a tileset reference.
 */
struct TilesetRef final
{
  /** The associated tileset definition. */
  Tileset tileset;

  /** The first tile identifier associated with the tileset. */
  TileID first_tile_id;

  [[nodiscard]]
  auto operator==(const TilesetRef&) const -> bool = default;
};

/**
 * Intermediate representation of a map tile format.
 */
struct TileFormat final
{
  /** The tile encoding strategy. */
  TileEncoding encoding;

  /** The tile compression strategy. */
  std::optional<CompressionFormatId> compression;

  /** The compression level. */
  std::optional<std::int32_t> compression_level;

  [[nodiscard]]
  auto operator==(const TileFormat&) const -> bool = default;
};

/**
 * Intermediate representation of a map.
 */
struct Map final
{
  /** Metadata for the map. */
  Metadata meta;

  /** The number of rows and columns in each tile layer. */
  Extent2D extent;

  /** The logical size of all tiles. */
  Int2 tile_size;

  /** The next available layer identifier. */
  LayerID next_layer_id;

  /** The next available object identifier. */
  ObjectID next_object_id;

  /** The tile format used by the map. */
  TileFormat tile_format;

  /** The associated component definitions. */
  std::vector<Component> components;

  /** The associated tilesets. */
  std::vector<TilesetRef> tilesets;

  /** The associated layers. */
  std::vector<Layer> layers;

  [[nodiscard]]
  auto operator==(const Map&) const -> bool = default;
};

}  // namespace tactile::ir
