// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/path.hpp"
#include "tactile/base/container/string.hpp"
#include "tactile/base/container/vector.hpp"
#include "tactile/base/id.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/base/util/chrono.hpp"
#include "tactile/core/io/compress/compression_type.hpp"
#include "tactile/core/layer/dense_tile_matrix.hpp"
#include "tactile/core/layer/layer_type.hpp"
#include "tactile/core/layer/object_type.hpp"
#include "tactile/core/meta/attribute.hpp"
#include "tactile/core/tile/tile_encoding.hpp"

namespace tactile::ir {

/// \addtogroup IO
/// \{

/**
 * Intermediate representation of an attribute with a given name.
 */
struct NamedAttribute final
{
  /** The attribute name. */
  String name;

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
  String name;

  /** The component attribute defaults. */
  Vector<NamedAttribute> attributes;

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
  String type;

  /** The component attributes. */
  Vector<NamedAttribute> attributes;

  [[nodiscard]]
  auto operator==(const AttachedComponent&) const -> bool = default;
};

/**
 * Provides the data featured by all meta contexts.
 */
struct Metadata final
{
  /** The attached properties. */
  Vector<NamedAttribute> properties;

  /** The attached components. */
  Vector<AttachedComponent> components;

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

  /** The object name. */
  String name;

  /** The associated identifier. */
  ObjectID id;

  /** The type of the object. */
  ObjectType type;

  /** The x-coordinate of the object. */
  float x;

  /** The y-coordinate of the object. */
  float y;

  /** The width of the object. */
  float width;

  /** The height of the object. */
  float height;

  /** A user-provided label. */
  String tag;

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
  Metadata meta;  ///<

  /** The layer name. */
  String name;

  /** The associated identifier. */
  int32 id;

  /** The type of the layer. */
  LayerType type;

  /** The opacity of the layer content. */
  float opacity;

  /** The number of tile rows (if tile layer). */
  ssize row_count;

  /** The number of tile columns (if tile layer). */
  ssize col_count;

  /** The contained tiles (if tile layer). */
  DenseTileMatrix tiles;

  /** The contained objects (if object layer). */
  Vector<Object> objects;

  /** The contained layers (if group layer). */
  Vector<Layer> layers;

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
  Milliseconds duration;

  [[nodiscard]]
  auto operator==(const AnimationFrame&) const -> bool = default;
};

using TileAnimation = Vector<AnimationFrame>;

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
  Vector<Object> objects;

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

  /** The tileset name. */
  String name;

  /** The width of tiles in the tileset. */
  int32 tile_width;

  /** The height of tiles in the tileset. */
  int32 tile_height;

  /** The total number of tiles in the tileset. */
  ssize tile_count;

  /** The number of tile columns in the tileset. */
  ssize column_count;

  /** The width of the associated image. */
  int32 image_width;

  /** The height of the associated image. */
  int32 image_height;

  /** The file path to the associated image. */
  Path image_path;

  /** The associated tile descriptors. */
  Vector<Tile> tiles;

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
  CompressionType compression;

  /** The compression level. */
  Maybe<int32> compression_level;

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

  /** The map name. */
  String name;

  /** The number of rows in each tile layer. */
  ssize row_count;

  /** The number of columns in each tile layer. */
  ssize col_count;

  /** The logical width of all tiles. */
  int32 tile_width;

  /** The logical height of all tiles. */
  int32 tile_height;

  /** The next available layer identifier. */
  int32 next_layer_id;

  /** The next available object identifier. */
  int32 next_object_id;

  /** The tile format used by the map. */
  TileFormat tile_format;

  /** The associated component definitions. */
  Vector<Component> components;

  /** The associated tilesets. */
  Vector<TilesetRef> tilesets;

  /** The associated layers. */
  Vector<Layer> layers;

  [[nodiscard]]
  auto operator==(const Map&) const -> bool = default;
};

/// \}

}  // namespace tactile::ir
