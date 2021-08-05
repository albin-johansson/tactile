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
#include "aliases/unique.hpp"
#include "aliases/tile_matrix.hpp"
#include "core/map/layers/layer_type.hpp"
#include "core/map/layers/object_type.hpp"
#include "core/properties/property.hpp"

namespace Tactile::IO {

struct PropertyData final
{
  std::string name;
  Property property;
};

struct TilesetData final
{
  tile_id first_id;
  int tile_width{};
  int tile_height{};
  std::filesystem::path absolute_image_path;
  std::string name;
  std::vector<PropertyData> properties;
};

struct TileLayerData final
{
  row_t row_count;
  col_t col_count;
  TileMatrix tiles;
};

struct ObjectData final
{
  object_id id;
  float x{};
  float y{};
  float width{};
  float height{};
  std::string custom_type;
  std::string name;
  std::vector<PropertyData> properties;
  bool visible{};
  ObjectType type{};
};

struct ObjectLayerData final
{
  std::vector<ObjectData> objects;
};

struct LayerData;

struct GroupLayerData final
{
  std::vector<Unique<LayerData>> layers;
};

struct LayerData final
{
  using LayerContent = std::variant<TileLayerData, ObjectLayerData, GroupLayerData>;

  layer_id id;
  LayerType type;
  LayerContent data;
  std::string name;
  std::vector<PropertyData> properties;
  float opacity{};
  bool is_visible{};
};

struct MapData final
{
  std::filesystem::path absolute_path;
  layer_id next_layer_id;
  object_id next_object_id;
  int tile_width{};
  int tile_height{};
  std::vector<TilesetData> tilesets;
  std::vector<LayerData> layers;
  std::vector<PropertyData> properties;
};

}  // namespace Tactile::IO
