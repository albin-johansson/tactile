#pragma once

#include <QString>  // QString
#include <variant>  // variant
#include <vector>   // vector

#include "layer.hpp"
#include "layer_id.hpp"
#include "object_id.hpp"
#include "position.hpp"
#include "property.hpp"
#include "tile_height.hpp"
#include "tile_id.hpp"
#include "tile_width.hpp"

namespace tactile::parse {

struct property_data final
{
  QString name;             ///< The name associated with the property.
  core::property property;  ///< The property value.
};

struct tileset_data final
{
  tile_id firstId;         ///< First tile ID used by the tileset.
  tile_width tileWidth;    ///< Tile width used by the tileset.
  tile_height tileHeight;  ///< Tile height used by the tileset.
  QString absolutePath;    ///< The absolute file path of the tileset.
  QString name;            ///< The name of the tileset.
};

struct tile_layer_data final
{
  row_t nRows;                              ///< Number of rows.
  col_t nCols;                              ///< Number of columns.
  std::vector<std::vector<tile_id>> tiles;  ///< The tile data.
};

struct object_data final
{
  object_id id;        ///< Unique ID associated with the object.
  double x;            ///< The x-coordinate of the object.
  double y;            ///< The y-coordinate of the object.
  double width;        ///< The width of the object.
  double height;       ///< The height of the object.
  QString customType;  ///< The custom type of the object.
  QString name;        ///< The name of the object.
  std::vector<property_data> properties;  ///< List of properties.
  bool visible;      ///< Indicates whether or not the object is visible.
  bool isPoint;      ///< Indicates whether or not the object is a point.
  bool isRectangle;  ///< Indicates whether or not the object is a rectangle.
};

struct object_layer_data final
{
  std::vector<object_data> objects;  ///< The objects present in the layer.
};

struct layer_data final
{
  using layer_content_data = std::variant<tile_layer_data, object_layer_data>;

  layer_id id;                            ///< Unique ID of the layer.
  core::layer_type type;                  ///< The type of the layer.
  layer_content_data data;                ///< Type specific data.
  QString name;                           ///< The name of the layer.
  std::vector<property_data> properties;  ///< The list of properties.
  double opacity;                         ///< The opacity of the layer.
  bool visible;  ///< Indicates whether or not the layer is visible.
};

struct map_data final
{
  layer_id nextLayerId;                   ///< Next unique ID for layers.
  object_id nextObjectId;                 ///< Next unique ID for objects.
  std::vector<tileset_data> tilesets;     ///< The tilesets used by the map.
  std::vector<layer_data> layers;         ///< The layers in the map.
  std::vector<property_data> properties;  ///< The map properties.
};

}  // namespace tactile::parse
