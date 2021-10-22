#pragma once

#include <filesystem>  // path
#include <memory>      // unique_ptr
#include <string>      // string
#include <variant>     // variant
#include <vector>      // vector

#include <tactile-base/layer_type.hpp>
#include <tactile-base/object_type.hpp>
#include <tactile-base/property_value.hpp>
#include <tactile-base/tactile_std.hpp>

namespace Tactile::IO {

/// \addtogroup io
/// \{

/* TODO: do not use Tactile::PropertyValue or cen::color */

/// \brief Intermediate representation of a property.
struct PropertyData final
{
  std::string name;     ///< The unique (within the context) property name
  PropertyValue value;  ///< The property value.
};

/// \brief Intermediate representation of a map object.
struct ObjectData final
{
  int32 id{};                            ///< Unique object identifier.
  float x{};                             ///< Logical x-coordinate.
  float y{};                             ///< Logical y-coordinate.
  float width{};                         ///< Logical width.
  float height{};                        ///< Logical height.
  ObjectType type{};                     ///< Specific object type.
  std::string tag;                       ///< Optional user-defined type tag.
  std::string name;                      ///< Object name.
  std::vector<PropertyData> properties;  ///< Object properties.
  bool visible{};                        ///< Is the object visible?
};

/// \brief Intermediate representation of a tile animation frame.
struct FrameData final
{
  int32 tile{};      ///< Local ID of the tile that should be displayed during the frame.
  int32 duration{};  ///< Frame duration in milliseconds.
};

/// \brief Intermediate representation of tile data.
struct TileData final
{
  int32 id{};                            ///< Local ID of the associated tile.
  std::vector<FrameData> animation;      ///< Optional animation frames.
  std::vector<ObjectData> objects;       ///< Optional collection of contained objects.
  std::vector<PropertyData> properties;  ///< Tile properties.
};

/// \brief Intermediate representation of a tileset.
struct TilesetData final
{
  int32 first_id{};                           ///< The first global tile ID.
  int32 tile_width{};                         ///< Logical tile width.
  int32 tile_height{};                        ///< Logical tile height.
  int32 tile_count{};                         ///< Total amount of tiles.
  int32 column_count{};                       ///< The amount of columns.
  int32 image_width{};                        ///< Width of the tileset image, in pixels.
  int32 image_height{};                       ///< Height of the tileset image, in pixels.
  std::filesystem::path absolute_image_path;  ///< Absolute path of tileset image.
  std::string name;                           ///< Tileset name.
  std::vector<TileData> tiles;                ///< Data related to specific tiles.
  std::vector<PropertyData> properties;       ///< Tileset properties.
};

/// \brief Intermediate representation of tile layer data.
struct TileLayerData final
{
  int32 row_count{};  ///< Total amount of rows.
  int32 col_count{};  ///< Total amount of columns.
  TileMatrix tiles;   ///< The associated tile data.
};

/// \brief Intermediate representation of object layer data.
struct ObjectLayerData final
{
  std::vector<ObjectData> objects;  ///< The associated objects.
};

struct LayerData;

/// \brief Intermediate representation of group layer data.
struct GroupLayerData final
{
  std::vector<std::unique_ptr<LayerData>> layers;  ///< Child layers in the group.
};

/// \brief Intermediate representation of a layer.
struct LayerData final
{
  using LayerContent = std::variant<TileLayerData, ObjectLayerData, GroupLayerData>;

  int32 id{};                            ///< Unique layer identifier.
  usize index{};                         ///< Local layer stack index.
  LayerType type{};                      ///< The type of the layer.
  LayerContent data;                     ///< Type-specific data.
  std::string name;                      ///< The name of the layer.
  std::vector<PropertyData> properties;  ///< The layer properties.
  float opacity{};                       ///< Opacity of the layer, [0, 1].
  bool is_visible{};                     ///< Is the layer visible?
};

/// \brief Intermediate representation of a map.
struct MapData final
{
  std::filesystem::path absolute_path;   ///< Absolute path of the map file.
  int32 next_layer_id{};                 ///< The next available layer ID.
  int32 next_object_id{};                ///< The next available object ID.
  int32 tile_width{};                    ///< The logical tile width.
  int32 tile_height{};                   ///< The logical tile height.
  int32 row_count{};                     ///< The number of rows.
  int32 column_count{};                  ///< The number of columns.
  std::vector<TilesetData> tilesets;     ///< The associated tilesets.
  std::vector<LayerData> layers;         ///< The associated layers.
  std::vector<PropertyData> properties;  ///< The map properties.
};

/// \} End of group io

}  // namespace Tactile::IO
