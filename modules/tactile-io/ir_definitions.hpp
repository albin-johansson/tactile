#pragma once

#include <filesystem>  // path
#include <memory>      // unique_ptr
#include <string>      // string
#include <variant>     // variant
#include <vector>      // vector

#include <tactile_def.hpp>

#include "ir_common.hpp"

/* This is a private header, and the following intermediate representation is subject to
   change at any point in time without any notice. The public APIs have been designed to
   provide a robust and stable ABI, but do not rely on the implementation details in any
   way in client code. You have been warned... :) */

namespace Tactile::IO {

/* Simple strong integral type for use in property value variant */
enum ObjectRef : int32 {};

/// \brief Intermediate representation of a property.
struct Property final {
  using Value = std::
      variant<std::string, int32, float, bool, Color, std::filesystem::path, ObjectRef>;

  std::string name;  ///< The unique (within the context) property name
  Value value;       ///< The property value.
};

/// \brief Intermediate representation of a map object.
struct Object final {
  int32 id{};                              ///< Unique object identifier.
  float x{};                               ///< Logical x-coordinate.
  float y{};                               ///< Logical y-coordinate.
  float width{};                           ///< Logical width.
  float height{};                          ///< Logical height.
  ObjectType type{ObjectType::Rectangle};  ///< Specific object type.
  std::string tag;                         ///< Optional user-defined type tag.
  std::string name;                        ///< Object name.
  std::vector<Property> properties;        ///< Object properties.
  bool visible{true};                      ///< Is the object visible?
};

/// \brief Intermediate representation of a tile animation frame.
struct AnimationFrame final {
  int32 tile{};      ///< Local ID of the tile that should be displayed during the frame.
  int32 duration{};  ///< Frame duration in milliseconds.
};

/// \brief Intermediate representation of tile data.
struct Tile final {
  int32 id{};                             ///< Local ID of the associated tile.
  std::vector<AnimationFrame> animation;  ///< Optional animation frames.
  std::vector<Object> objects;            ///< Optional collection of contained objects.
  std::vector<Property> properties;       ///< Tile properties.
};

/// \brief Intermediate representation of a tileset.
struct Tileset final {
  int32 first_id{};                           ///< The first global tile ID.
  int32 tile_width{};                         ///< Logical tile width.
  int32 tile_height{};                        ///< Logical tile height.
  int32 tile_count{};                         ///< Total amount of tiles.
  int32 column_count{};                       ///< The amount of columns.
  int32 image_width{};                        ///< Width of the tileset image, in pixels.
  int32 image_height{};                       ///< Height of the tileset image, in pixels.
  std::filesystem::path absolute_image_path;  ///< Absolute path of tileset image.
  std::string name;                           ///< Tileset name.
  std::vector<Tile> tiles;                    ///< Data related to specific tiles.
  std::vector<Property> properties;           ///< Tileset properties.
};

/// \brief Intermediate representation of tile layer data.
struct TileLayer final {
  usize row_count{};  ///< Total amount of rows.
  usize col_count{};  ///< Total amount of columns.
  TileMatrix tiles;   ///< The associated tile data.
};

/// \brief Intermediate representation of object layer data.
struct ObjectLayer final {
  std::vector<Object> objects;  ///< The associated objects.
};

struct Layer;

/// \brief Intermediate representation of group layer data.
struct GroupLayer final {
  std::vector<std::unique_ptr<Layer>> layers;  ///< Child layers in the group.
};

/// \brief Intermediate representation of a layer.
struct Layer final {
  using LayerContent = std::variant<TileLayer, ObjectLayer, GroupLayer>;

  int32 id{};                        ///< Unique layer identifier.
  usize index{};                     ///< Local layer stack index.
  LayerType type{};                  ///< The type of the layer.
  LayerContent data;                 ///< Type-specific data.
  std::string name;                  ///< The name of the layer.
  std::vector<Property> properties;  ///< The layer properties.
  float opacity{1.0f};               ///< Opacity of the layer, [0, 1].
  bool is_visible{true};             ///< Is the layer visible?
};

/// \brief Intermediate representation of a map.
struct Map final {
  std::filesystem::path absolute_path;  ///< Absolute path of the map file.
  int32 next_layer_id{};                ///< The next available layer ID.
  int32 next_object_id{};               ///< The next available object ID.
  int32 tile_width{};                   ///< The logical tile width.
  int32 tile_height{};                  ///< The logical tile height.
  usize row_count{};                    ///< The number of rows.
  usize column_count{};                 ///< The number of columns.
  std::vector<Tileset> tilesets;        ///< The associated tilesets.
  std::vector<Layer> layers;            ///< The associated layers.
  std::vector<Property> properties;     ///< The map properties.
};

}  // namespace Tactile::IO
