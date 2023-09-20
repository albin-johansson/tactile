// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/container/string.hpp"
#include "tactile/core/container/variant.hpp"
#include "tactile/core/container/vector.hpp"
#include "tactile/core/context/attribute.hpp"
#include "tactile/core/io/filesystem.hpp"
#include "tactile/core/map/layer/layer_type.hpp"
#include "tactile/core/map/layer/object_type.hpp"
#include "tactile/core/map/tile_compression.hpp"
#include "tactile/core/map/tile_encoding.hpp"
#include "tactile/core/prelude.hpp"

namespace tactile::ir {

/**
 * \brief Intermediate representation of an attribute with a given name.
 */
struct NamedAttribute final {
  String name;      ///< The attribute name.
  Attribute value;  ///< The attribute value.
};

/**
 * \brief Intermediate representation of a component definition.
 */
struct Component final {
  String name;                        ///< The component name.
  Vector<NamedAttribute> attributes;  ///< The component attribute defaults.
};

/**
 * \brief Intermediate representation of a component instance, attached to a meta context.
 */
struct AttachedComponent final {
  String name;                        ///< The component type name.
  Vector<NamedAttribute> attributes;  ///< The component attributes.
};

/**
 * \brief Provides the data featured by all meta contexts.
 */
struct Metadata final {
  String name;                           ///< The context name.
  Vector<NamedAttribute> properties;     ///< The attached properties.
  Vector<AttachedComponent> components;  ///< The attached components.
};

/**
 * \brief Intermediate representation of a layer object.
 */
struct Object final {
  Metadata meta;    ///< Metadata for the object.
  ObjectID id;      ///< The associated identifier.
  ObjectType type;  ///< The type of the object.
  float32 x;        ///< The x-coordinate of the object.
  float32 y;        ///< The y-coordinate of the object.
  float32 width;    ///< The width of the object.
  float32 height;   ///< The height of the object.
  String tag;       ///< A user-provided label.
  bool visible;     ///< Whether the object is rendered.
};

/**
 * \brief Intermediate representation of a layer.
 */
struct Layer final {
  Metadata meta;                ///< Metadata for the layer.
  int32 id;                     ///< The associated identifier.
  LayerType type;               ///< The type of the layer.
  float32 opacity;              ///< The opacity of the layer content.
  Vector<Vector<int32>> tiles;  ///< The contained tiles (if tile layer).
  Vector<Object> objects;       ///< The contained objects (if object layer).
  Vector<Layer> layers;         ///< The contained layers (if group layer).
  bool visible;                 ///< Whether the layer is rendered.
};

/**
 * \brief Intermediate representation of a frame in a tile animation.
 */
struct AnimationFrame final {
  int32 tile_index;    ///< The index of the rendered tile in the associated tileset.
  uint64 duration_ms;  ///< The duration that the frame is shown in milliseconds.
};

/**
 * \brief Intermediate representation of a tile definition.
 */
struct Tile final {
  Metadata meta;                     ///< Metadata for the tile.
  Vector<Object> objects;            ///< The contained objects, if any.
  Vector<AnimationFrame> animation;  ///< The associated animation frames, if any.
};

/**
 * \brief Intermediate representation of a tileset definition.
 */
struct Tileset final {
  Metadata meta;        ///< Metadata for the tileset.
  int32 tile_width;     ///< The width of tiles in the tileset.
  int32 tile_height;    ///< The height of tiles in the tileset.
  usize row_count;      ///< The number of tile rows in the tileset.
  usize col_count;      ///< The number of tile columns in the tileset.
  int32 image_width;    ///< The width of the associated image.
  int32 image_height;   ///< The height of the associated image.
  fs::Path image_path;  ///< The file path to the associated image.
  Vector<Tile> tiles;   ///< The associated tile descriptors.
};

/**
 * \brief Intermediate representation of a tileset reference.
 */
struct TilesetRef final {
  Tileset tileset;      ///< The tileset definition.
  int32 first_tile_id;  ///< The first global tile identifier associated with the tileset.
};

/**
 * \brief Intermediate representation of a map tile format.
 */
struct TileFormat final {
  TileEncoding encoding;        ///< The tile encoding strategy.
  TileCompression compression;  ///< The tile compression strategy.
  int32 zlib_level;             ///< The compression level (if using Zlib).
  int32 zstd_level;             ///< The compression level (if using Zstd).
};

/**
 * \brief Intermediate representation of a map.
 */
struct Map final {
  Metadata meta;                 ///< Metadata for the map.
  usize row_count;               ///< The number of rows in each tile layer.
  usize col_count;               ///< The number of columns in each tile layer.
  int32 tile_width;              ///< The logical width of all tiles.
  int32 tile_height;             ///< The logical height of all tiles.
  int32 next_layer_id;           ///< The next available layer identifier.
  int32 next_object_id;          ///< The next available object identifier.
  Vector<Component> components;  ///< The associated component definitions.
  Vector<Tileset> tilesets;      ///< The associated tilesets.
  Vector<Layer> layers;          ///< The associated layers.
};

}  // namespace tactile::ir
