// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/container/string.hpp"
#include "tactile/core/container/variant.hpp"
#include "tactile/core/container/vector.hpp"
#include "tactile/core/context/attribute.hpp"
#include "tactile/core/functional/maybe.hpp"
#include "tactile/core/io/compression/compression.hpp"
#include "tactile/core/io/filesystem.hpp"
#include "tactile/core/map/layer/layer_type.hpp"
#include "tactile/core/map/layer/object_type.hpp"
#include "tactile/core/map/tile_encoding.hpp"
#include "tactile/core/misc/id_types.hpp"
#include "tactile/core/misc/tile_matrix.hpp"
#include "tactile/core/prelude.hpp"

namespace tactile::ir {

/**
 * \brief Intermediate representation of an attribute with a given name.
 */
struct NamedAttribute final {
  String name;      ///< The attribute name.
  Attribute value;  ///< The attribute value.

  [[nodiscard]] auto operator==(const NamedAttribute&) const -> bool = default;
};

/**
 * \brief Intermediate representation of a component definition.
 */
struct Component final {
  String name;                        ///< The component name.
  Vector<NamedAttribute> attributes;  ///< The component attribute defaults.

  [[nodiscard]] auto operator==(const Component&) const -> bool = default;
};

/**
 * \brief Intermediate representation of a component instance, attached to a meta context.
 */
struct AttachedComponent final {
  String type;                        ///< The component type name.
  Vector<NamedAttribute> attributes;  ///< The component attributes.

  [[nodiscard]] auto operator==(const AttachedComponent&) const -> bool = default;
};

/**
 * \brief Provides the data featured by all meta contexts.
 */
struct Metadata final {
  String name;                           ///< The context name.
  Vector<NamedAttribute> properties;     ///< The attached properties.
  Vector<AttachedComponent> components;  ///< The attached components.

  [[nodiscard]] auto operator==(const Metadata&) const -> bool = default;
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

  [[nodiscard]] auto operator==(const Object&) const -> bool = default;
};

/**
 * \brief Intermediate representation of a layer.
 * \todo Make width/height type usize
 */
struct Layer final {
  Metadata meta;           ///< Metadata for the layer.
  int32 id;                ///< The associated identifier.
  LayerType type;          ///< The type of the layer.
  float32 opacity;         ///< The opacity of the layer content.
  usize width;             ///< The number of tile columns (if tile layer).
  usize height;            ///< The number of tile rows (if tile layer).
  TileMatrix tiles;        ///< The contained tiles (if tile layer).
  Vector<Object> objects;  ///< The contained objects (if object layer).
  Vector<Layer> layers;    ///< The contained layers (if group layer).
  bool visible {};         ///< Whether the layer is rendered.

  [[nodiscard]] auto operator==(const Layer&) const -> bool = default;
};

/**
 * \brief Intermediate representation of a frame in a tile animation.
 */
struct AnimationFrame final {
  TileIndex tile_index;  ///< The index of the rendered tile in the associated tileset.
  uint64 duration_ms;    ///< The duration that the frame is shown in milliseconds.

  [[nodiscard]] auto operator==(const AnimationFrame&) const -> bool = default;
};

using TileAnimation = Vector<AnimationFrame>;

/**
 * \brief Intermediate representation of a tile definition.
 */
struct Tile final {
  Metadata meta;            ///< Metadata for the tile.
  TileIndex index;          ///< The index in the associated tileset.
  Vector<Object> objects;   ///< The contained objects, if any.
  TileAnimation animation;  ///< The associated animation frames, if any.

  [[nodiscard]] auto operator==(const Tile&) const -> bool = default;
};

/**
 * \brief Intermediate representation of a tileset definition.
 */
struct Tileset final {
  Metadata meta;        ///< Metadata for the tileset.
  int32 tile_width;     ///< The width of tiles in the tileset.
  int32 tile_height;    ///< The height of tiles in the tileset.
  usize tile_count;     ///< The total number of tiles in the tileset.
  usize column_count;   ///< The number of tile columns in the tileset.
  int32 image_width;    ///< The width of the associated image.
  int32 image_height;   ///< The height of the associated image.
  FilePath image_path;  ///< The file path to the associated image.
  Vector<Tile> tiles;   ///< The associated tile descriptors.

  [[nodiscard]] auto operator==(const Tileset&) const -> bool = default;
};

/**
 * \brief Intermediate representation of a tileset reference.
 */
struct TilesetRef final {
  Tileset tileset;       ///< The tileset definition.
  TileID first_tile_id;  ///< The first tile identifier associated with the tileset.

  [[nodiscard]] auto operator==(const TilesetRef&) const -> bool = default;
};

/**
 * \brief Intermediate representation of a map tile format.
 */
struct TileFormat final {
  TileEncoding encoding;        ///< The tile encoding strategy.
  CompressionMode compression;  ///< The tile compression strategy.
  Maybe<int32> zlib_level;      ///< The compression level (if using Zlib).
  Maybe<int32> zstd_level;      ///< The compression level (if using Zstd).

  [[nodiscard]] auto operator==(const TileFormat&) const -> bool = default;
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
  TileFormat tile_format;        ///< The tile format used by the map.
  Vector<Component> components;  ///< The associated component definitions.
  Vector<TilesetRef> tilesets;   ///< The associated tilesets.
  Vector<Layer> layers;          ///< The associated layers.

  [[nodiscard]] auto operator==(const Map&) const -> bool = default;
};

}  // namespace tactile::ir
