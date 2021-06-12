#pragma once

#include <QString>  // QString
#include <variant>  // variant
#include <vector>   // vector

#include "layer.hpp"
#include "layer_id.hpp"
#include "map_position.hpp"
#include "object_id.hpp"
#include "property.hpp"
#include "tile_height.hpp"
#include "tile_id.hpp"
#include "tile_layer.hpp"
#include "tile_width.hpp"

namespace tactile::ir {

struct PropertyData final
{
  QString name;             ///< The name associated with the property.
  core::Property property;  ///< The property value.
};

struct TilesetData final
{
  tile_id firstId;         ///< First tile ID used by the tileset.
  tile_width tileWidth;    ///< Tile width used by the tileset.
  tile_height tileHeight;  ///< Tile height used by the tileset.
  QString absolutePath;    ///< The absolute file path of the tileset.
  QString name;            ///< The name of the tileset.
  std::vector<PropertyData> properties;  ///< List of properties.
};

struct TileLayerData final
{
  row_t nRows;              ///< Number of rows.
  col_t nCols;              ///< Number of columns.
  core::tile_matrix tiles;  ///< The tile data.
};

struct ObjectData final
{
  object_id id;        ///< Unique ID associated with the object.
  double x{};          ///< The x-coordinate of the object.
  double y{};          ///< The y-coordinate of the object.
  double width{};      ///< The width of the object.
  double height{};     ///< The height of the object.
  QString customType;  ///< The custom type of the object.
  QString name;        ///< The name of the object.
  std::vector<PropertyData> properties;  ///< List of properties.
  bool visible{};      ///< Indicates whether or not the object is visible.
  bool isPoint{};      ///< Indicates whether or not the object is a point.
  bool isRectangle{};  ///< Indicates whether or not the object is a rectangle.
};

struct ObjectLayerData final
{
  std::vector<ObjectData> objects;  ///< The objects present in the layer.
};

struct LayerData final
{
  using layer_content_data = std::variant<TileLayerData, ObjectLayerData>;

  layer_id id;                           ///< Unique ID of the layer.
  core::LayerType type;                  ///< The type of the layer.
  layer_content_data data;               ///< Type specific data.
  QString name;                          ///< The name of the layer.
  std::vector<PropertyData> properties;  ///< The list of properties.
  double opacity{};                      ///< The opacity of the layer.
  bool visible{};  ///< Indicates whether or not the layer is visible.
};

struct MapData final
{
  layer_id nextLayerId;                  ///< Next unique ID for layers.
  object_id nextObjectId;                ///< Next unique ID for objects.
  std::vector<TilesetData> tilesets;     ///< The tilesets used by the map.
  std::vector<LayerData> layers;         ///< The layers in the map.
  std::vector<PropertyData> properties;  ///< The map properties.
  QString path;                          ///< Absolute file path.
};

}  // namespace tactile::ir
