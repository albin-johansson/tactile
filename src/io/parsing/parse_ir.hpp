#pragma once

#include <filesystem>  // path
#include <string>      // string
#include <variant>     // variant
#include <vector>      // vector

#include "aliases/col.hpp"
#include "aliases/layer_id.hpp"
#include "aliases/object_id.hpp"
#include "aliases/row.hpp"
#include "aliases/tile_id.hpp"
#include "aliases/tile_matrix.hpp"
#include "aliases/unique.hpp"
#include "core/map/layers/layer_type.hpp"
#include "core/map/layers/object_type.hpp"
#include "core/properties/property.hpp"

namespace Tactile::IO {

/// \addtogroup io
/// \{

/**
 * \brief Intermediate representation of a property.
 */
struct PropertyData final
{
  std::string name;   ///< The unique (within the context) property name
  Property property;  ///< The property value.
};

/**
 * \brief Intermediate representation of a tile animation frame.
 */
struct FrameData final
{
  tile_id tile;    ///< The tile that should be displayed during the frame.
  int duration{};  ///< Frame duration in milliseconds.
};

/**
 * \brief Intermediate representation of tile data.
 */
struct TileData final
{
  tile_id id;                        ///< The associated tile.
  std::vector<FrameData> animation;  ///< Optional animation frames.
  // TODO properties
};

/**
 * \brief Intermediate representation of a tileset.
 */
struct TilesetData final
{
  tile_id first_id;                           ///< The first global tile ID.
  int tile_width{};                           ///< Logical tile width.
  int tile_height{};                          ///< Logical tile height.
  std::filesystem::path absolute_image_path;  ///< Absolute path of tileset image.
  std::string name;                           ///< Tileset name.
  std::vector<TileData> tiles;                ///< Data related to specific tiles.
  std::vector<PropertyData> properties;       ///< Tileset properties.
};

/**
 * \brief Intermediate representation of tile layer data.
 */
struct TileLayerData final
{
  row_t row_count;   ///< Total amount of rows.
  col_t col_count;   ///< Total amount of columns.
  TileMatrix tiles;  ///< The associated tile data.
};

/**
 * \brief Intermediate representation of a map object.
 */
struct ObjectData final
{
  object_id id;                          ///< Unique object identifier.
  float x{};                             ///< Logical x-coordinate.
  float y{};                             ///< Logical y-coordinate.
  float width{};                         ///< Logical width.
  float height{};                        ///< Logical height.
  ObjectType type{};                     ///< Specific object type.
  std::string custom_type;               ///< Optional custom type string.
  std::string name;                      ///< Object name.
  std::vector<PropertyData> properties;  ///< Object properties.
  bool visible{};                        ///< Is the object visible?
};

/**
 * \brief Intermediate representation of object layer data.
 */
struct ObjectLayerData final
{
  std::vector<ObjectData> objects;  ///< The associated objects.
};

struct LayerData;

/**
 * \brief Intermediate representation of group layer data.
 */
struct GroupLayerData final
{
  std::vector<Unique<LayerData>> layers;  ///< Child layers in the group.
};

/**
 * \brief Intermediate representation of a layer.
 */
struct LayerData final
{
  using LayerContent = std::variant<TileLayerData, ObjectLayerData, GroupLayerData>;

  layer_id id;                           ///< Unique layer identifier.
  LayerType type;                        ///< The type of the layer.
  LayerContent data;                     ///< Type-specific data.
  std::string name;                      ///< The name of the layer.
  std::vector<PropertyData> properties;  ///< The layer properties.
  float opacity{};                       ///< Opacity of the layer, [0, 1].
  bool is_visible{};                     ///< Is the layer visible?
};

/**
 * \brief Intermediate representation of a map.
 */
struct MapData final
{
  std::filesystem::path absolute_path;   ///< Absolute path of the map file.
  layer_id next_layer_id;                ///< The next available layer ID.
  object_id next_object_id;              ///< The next available object ID.
  int tile_width{};                      ///< The logical tile width.
  int tile_height{};                     ///< The logical tile height.
  std::vector<TilesetData> tilesets;     ///< The associated tilesets.
  std::vector<LayerData> layers;         ///< The associated layers.
  std::vector<PropertyData> properties;  ///< The map properties.
};

/// \} End of group io

}  // namespace Tactile::IO
