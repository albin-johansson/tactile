#include "parse_ir.hpp"

#include <cassert>     // assert
#include <filesystem>  // path, absolute
#include <string>      // string
#include <variant>     // variant
#include <vector>      // vector

#include <tactile-base/property_value.hpp>

namespace Tactile::IO {

/// \brief Intermediate representation of a property.
struct Property final
{
  std::string name;     ///< The unique (within the context) property name
  PropertyValue value;  ///< The property value.
};

/// \brief Intermediate representation of a map object.
struct Object final
{
  int32 id{};                        ///< Unique object identifier.
  float x{};                         ///< Logical x-coordinate.
  float y{};                         ///< Logical y-coordinate.
  float width{};                     ///< Logical width.
  float height{};                    ///< Logical height.
  ObjectType type{};                 ///< Specific object type.
  std::string tag;                   ///< Optional user-defined type tag.
  std::string name;                  ///< Object name.
  std::vector<Property> properties;  ///< Object properties.
  bool visible{};                    ///< Is the object visible?
};

/// \brief Intermediate representation of a tile animation frame.
struct AnimationFrame final
{
  int32 tile{};      ///< Local ID of the tile that should be displayed during the frame.
  int32 duration{};  ///< Frame duration in milliseconds.
};

/// \brief Intermediate representation of tile data.
struct Tile final
{
  int32 id{};                             ///< Local ID of the associated tile.
  std::vector<AnimationFrame> animation;  ///< Optional animation frames.
  std::vector<Object> objects;            ///< Optional collection of contained objects.
  std::vector<Property> properties;       ///< Tile properties.
};

/// \brief Intermediate representation of a tileset.
struct Tileset final
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
  std::vector<Tile> tiles;                    ///< Data related to specific tiles.
  std::vector<Property> properties;           ///< Tileset properties.
};

/// \brief Intermediate representation of tile layer data.
struct TileLayer final
{
  int32 row_count{};  ///< Total amount of rows.
  int32 col_count{};  ///< Total amount of columns.
  TileMatrix tiles;   ///< The associated tile data.
};

/// \brief Intermediate representation of object layer data.
struct ObjectLayer final
{
  std::vector<Object> objects;  ///< The associated objects.
};

struct Layer;

/// \brief Intermediate representation of group layer data.
struct GroupLayer final
{
  std::vector<std::unique_ptr<Layer>> layers;  ///< Child layers in the group.
};

/// \brief Intermediate representation of a layer.
struct Layer final
{
  using LayerContent = std::variant<TileLayer, ObjectLayer, GroupLayer>;

  int32 id{};                        ///< Unique layer identifier.
  usize index{};                     ///< Local layer stack index.
  LayerType type{};                  ///< The type of the layer.
  LayerContent data;                 ///< Type-specific data.
  std::string name;                  ///< The name of the layer.
  std::vector<Property> properties;  ///< The layer properties.
  float opacity{};                   ///< Opacity of the layer, [0, 1].
  bool is_visible{};                 ///< Is the layer visible?
};

/// \brief Intermediate representation of a map.
struct Map final
{
  std::filesystem::path absolute_path;  ///< Absolute path of the map file.
  int32 next_layer_id{};                ///< The next available layer ID.
  int32 next_object_id{};               ///< The next available object ID.
  int32 tile_width{};                   ///< The logical tile width.
  int32 tile_height{};                  ///< The logical tile height.
  int32 row_count{};                    ///< The number of rows.
  int32 column_count{};                 ///< The number of columns.
  std::vector<Tileset> tilesets;        ///< The associated tilesets.
  std::vector<Layer> layers;            ///< The associated layers.
  std::vector<Property> properties;     ///< The map properties.
};

auto NewMap() -> Map*
{
  return new Map{};
}

void DeleteMap(Map* map) noexcept
{
  delete map;
}

void SetPath(Map& map, const CStr path)
{
  assert(path);
  map.absolute_path = std::filesystem::absolute(path);
}

void SetPath(Map& map, const CPathStr path)
{
  assert(path);
  map.absolute_path = std::filesystem::absolute(path);
}

void SetNextLayerId(Map& map, const int32 id)
{
  map.next_layer_id = id;
}

void SetNextObjectId(Map& map, const int32 id)
{
  map.next_object_id = id;
}

void SetTileWidth(Map& map, const int32 width)
{
  assert(width > 0);
  map.tile_width = width;
}

void SetTileHeight(Map& map, const int32 height)
{
  assert(height > 0);
  map.tile_height = height;
}

void SetRowCount(Map& map, const int32 rows)
{
  assert(rows > 0);
  map.row_count = rows;
}

void SetColumnCount(Map& map, const int32 columns)
{
  assert(columns > 0);
  map.column_count = columns;
}

void ReserveTilesets(Map& map, const usize n)
{
  map.tilesets.reserve(n);
}

void ReserveLayers(Map& map, const usize n)
{
  map.layers.reserve(n);
}

void ReserveProperties(Map& map, const usize n)
{
  map.properties.reserve(n);
}

auto AddTileset(Map& map) -> Tileset&
{
  return map.tilesets.emplace_back();
}

auto AddLayer(Map& map) -> Layer&
{
  return map.layers.emplace_back();
}

auto AddProperty(Map& map) -> Property&
{
  return map.properties.emplace_back();
}

auto GetAbsolutePath(const Map& map) -> CPathStr
{
  return map.absolute_path.c_str();
}

auto GetNextLayerId(const Map& map) -> int32
{
  return map.next_layer_id;
}

auto GetNextObjectId(const Map& map) -> int32
{
  return map.next_object_id;
}

auto GetTileWidth(const Map& map) -> int32
{
  return map.tile_width;
}

auto GetTileHeight(const Map& map) -> int32
{
  return map.tile_height;
}

auto GetRowCount(const Map& map) -> int32
{
  return map.row_count;
}

auto GetColumnCount(const Map& map) -> int32
{
  return map.column_count;
}

auto GetTilesetCount(const Map& map) -> usize
{
  return map.tilesets.size();
}

auto GetLayerCount(const Map& map) -> usize
{
  return map.layers.size();
}

auto GetPropertyCount(const Map& map) -> usize
{
  return map.properties.size();
}

auto GetTileset(const Map& map, const usize index) -> const Tileset&
{
  return map.tilesets.at(index);
}

auto GetLayer(const Map& map, const usize index) -> const Layer&
{
  return map.layers.at(index);
}

auto GetProperty(const Map& map, const usize index) -> const Property&
{
  return map.properties.at(index);
}

void SetName(Tileset& tileset, const CStr name)
{
  assert(name);
  tileset.name = name;
}

void SetFirstGlobalId(Tileset& tileset, const int32 id)
{
  tileset.first_id = id;
}

void SetTileWidth(Tileset& tileset, const int32 width)
{
  assert(width > 0);
  tileset.tile_width = width;
}

void SetTileHeight(Tileset& tileset, const int32 height)
{
  assert(height > 0);
  tileset.tile_height = height;
}

void SetTileCount(Tileset& tileset, const int32 count)
{
  assert(count > 0);
  tileset.tile_count = count;
}

void SetColumnCount(Tileset& tileset, const int32 columns)
{
  assert(columns > 0);
  tileset.column_count = columns;
}

void SetImagePath(Tileset& tileset, const CPathStr path)
{
  assert(path);
  tileset.absolute_image_path = std::filesystem::absolute(path);
}

void SetImageWidth(Tileset& tileset, const int32 width)
{
  assert(width > 0);
  tileset.image_width = width;
}

void SetImageHeight(Tileset& tileset, const int32 height)
{
  assert(height > 0);
  tileset.image_height = height;
}

void ReserveTiles(Tileset& tileset, const usize n)
{
  tileset.tiles.reserve(n);
}

void ReserveProperties(Tileset& tileset, const usize n)
{
  tileset.properties.reserve(n);
}

auto AddTile(Tileset& tileset) -> Tile&
{
  return tileset.tiles.emplace_back();
}

auto AddProperty(Tileset& tileset) -> Property&
{
  return tileset.properties.emplace_back();
}

auto GetName(const Tileset& tileset) -> CStr
{
  return tileset.name.c_str();
}

auto GetFirstGlobalId(const Tileset& tileset) -> int32
{
  return tileset.first_id;
}

auto GetTileWidth(const Tileset& tileset) -> int32
{
  return tileset.tile_width;
}

auto GetTileHeight(const Tileset& tileset) -> int32
{
  return tileset.tile_height;
}

auto GetTileCount(const Tileset& tileset) -> int32
{
  return tileset.tile_count;
}

auto GetColumnCount(const Tileset& tileset) -> int32
{
  return tileset.column_count;
}

auto GetImagePath(const Tileset& tileset) -> CPathStr
{
  return tileset.absolute_image_path.c_str();
}

auto GetImageWidth(const Tileset& tileset) -> int32
{
  return tileset.image_width;
}

auto GetImageHeight(const Tileset& tileset) -> int32
{
  return tileset.image_height;
}

auto GetTileInfoCount(const Tileset& tileset) -> usize
{
  return tileset.tiles.size();
}

auto GetTileInfo(const Tileset& tileset, const usize index) -> const Tile&
{
  return tileset.tiles.at(index);
}

auto GetPropertyCount(const Tileset& tileset) -> usize
{
  return tileset.properties.size();
}

auto GetProperty(const Tileset& tileset, const usize index) -> const Property&
{
  return tileset.properties.at(index);
}

void SetId(Tile& tile, const int32 id)
{
  tile.id = id;
}

void ReserveAnimationFrames(Tile& tile, const usize n)
{
  tile.animation.reserve(n);
}

void ReserveObjects(Tile& tile, const usize n)
{
  tile.objects.reserve(n);
}

void ReserveProperties(Tile& tile, const usize n)
{
  tile.properties.reserve(n);
}

auto AddAnimationFrame(Tile& tile) -> AnimationFrame&
{
  return tile.animation.emplace_back();
}

auto AddObject(Tile& tile) -> Object&
{
  return tile.objects.emplace_back();
}

auto AddProperty(Tile& tile) -> Property&
{
  return tile.properties.emplace_back();
}

auto IsTileWorthSaving(const Tile& tile) -> bool
{
  return !tile.animation.empty() || !tile.properties.empty() || !tile.objects.empty();
}

auto GetId(const Tile& tile) -> int32
{
  return tile.id;
}

auto GetAnimationFrameCount(const Tile& tile) -> usize
{
  return tile.animation.size();
}

auto GetObjectCount(const Tile& tile) -> usize
{
  return tile.objects.size();
}

auto GetPropertyCount(const Tile& tile) -> usize
{
  return tile.properties.size();
}

auto GetAnimationFrame(const Tile& tile, const usize index) -> const AnimationFrame&
{
  return tile.animation.at(index);
}

auto GetObject(const Tile& tile, const usize index) -> const Object&
{
  return tile.objects.at(index);
}

auto GetProperty(const Tile& tile, const usize index) -> const Property&
{
  return tile.properties.at(index);
}

void SetTile(AnimationFrame& frame, const int32 tile)
{
  frame.tile = tile;
}

void SetDuration(AnimationFrame& frame, const int32 ms)
{
  assert(ms >= 0);
  frame.duration = ms;
}

auto GetTile(const AnimationFrame& frame) -> int32
{
  return frame.tile;
}

auto GetDuration(const AnimationFrame& frame) -> int32
{
  return frame.duration;
}

void SetName(Object& object, const CStr name)
{
  assert(name);
  object.name = name;
}

void SetId(Object& object, const int32 id)
{
  object.id = id;
}

void SetX(Object& object, const float x)
{
  object.x = x;
}

void SetY(Object& object, const float y)
{
  object.y = y;
}

void SetWidth(Object& object, const float width)
{
  object.width = width;
}

void SetHeight(Object& object, const float height)
{
  object.height = height;
}

void SetType(Object& object, const ObjectType type)
{
  object.type = type;
}

void SetTag(Object& object, const CStr tag)
{
  assert(tag);
  object.tag = tag;
}

void SetVisible(Object& object, const bool visible)
{
  object.visible = visible;
}

void ReserveProperties(Object& object, const usize n)
{
  object.properties.reserve(n);
}

auto AddProperty(Object& object) -> Property&
{
  return object.properties.emplace_back();
}

auto GetPropertyCount(const Object& object) -> usize
{
  return object.properties.size();
}

auto GetProperty(const Object& object, const usize index) -> const Property&
{
  return object.properties.at(index);
}

auto GetName(const Object& object) -> CStr
{
  return object.name.c_str();
}

auto GetId(const Object& object) -> int32
{
  return object.id;
}

auto GetX(const Object& object) -> float
{
  return object.x;
}

auto GetY(const Object& object) -> float
{
  return object.y;
}

auto GetWidth(const Object& object) -> float
{
  return object.width;
}

auto GetHeight(const Object& object) -> float
{
  return object.height;
}

auto GetType(const Object& object) -> ObjectType
{
  return object.type;
}

auto GetTag(const Object& object) -> CStr
{
  return object.tag.c_str();
}

auto IsVisible(const Object& object) -> bool
{
  return object.visible;
}

auto MarkAsTileLayer(Layer& layer) -> TileLayer&
{
  return layer.data.emplace<TileLayer>();
}

auto MarkAsObjectLayer(Layer& layer) -> ObjectLayer&
{
  return layer.data.emplace<ObjectLayer>();
}

auto MarkAsGroupLayer(Layer& layer) -> GroupLayer&
{
  return layer.data.emplace<GroupLayer>();
}

auto GetTileLayer(const Layer& layer) -> const TileLayer&
{
  return std::get<TileLayer>(layer.data);
}

auto GetObjectLayer(const Layer& layer) -> const ObjectLayer&
{
  return std::get<ObjectLayer>(layer.data);
}

auto GetGroupLayer(const Layer& layer) -> const GroupLayer&
{
  return std::get<GroupLayer>(layer.data);
}

void SetId(Layer& layer, const int32 id)
{
  layer.id = id;
}

void SetIndex(Layer& layer, const usize index)
{
  layer.index = index;
}

void SetType(Layer& layer, const LayerType type)
{
  layer.type = type;
}

void SetName(Layer& layer, const CStr name)
{
  assert(name);
  layer.name = name;
}

void SetOpacity(Layer& layer, const float opacity)
{
  assert(opacity >= 0.0f);
  assert(opacity <= 1.0f);
  layer.opacity = opacity;
}

void SetVisible(Layer& layer, const bool visible)
{
  layer.is_visible = visible;
}

auto GetId(const Layer& layer) -> int32
{
  return layer.id;
}

auto GetIndex(const Layer& layer) -> usize
{
  return layer.index;
}

auto GetType(const Layer& layer) -> LayerType
{
  return layer.type;
}

auto GetName(const Layer& layer) -> CStr
{
  return layer.name.c_str();
}

auto GetOpacity(const Layer& layer) -> float
{
  return layer.opacity;
}

auto IsVisible(const Layer& layer) -> bool
{
  return layer.is_visible;
}

auto GetPropertyCount(const Layer& layer) -> usize
{
  return layer.properties.size();
}

auto GetProperty(const Layer& layer, const usize index) -> const Property&
{
  return layer.properties.at(index);
}

void ReserveProperties(Layer& layer, const usize n)
{
  layer.properties.reserve(n);
}

auto AddProperty(Layer& layer) -> Property&
{
  return layer.properties.emplace_back();
}

void ReserveTiles(TileLayer& layer, const int32 nRows, const int32 nCols)
{
  assert(nRows > 0);
  assert(nCols > 0);

  layer.row_count = nRows;
  layer.col_count = nCols;

  layer.tiles = MakeTileMatrix(nRows, nCols);
}

void SetTile(TileLayer& layer, const int32 row, const int32 column, const int32 tile)
{
  layer.tiles.at(row).at(column) = TileID{tile};
}

auto GetRowCount(const TileLayer& layer) -> int32
{
  return layer.row_count;
}

auto GetColumnCount(const TileLayer& layer) -> int32
{
  return layer.col_count;
}

auto GetTile(const TileLayer& layer, const int32 row, const int32 column) -> int32
{
  return layer.tiles.at(row).at(column).get();
}

void ReserveObjects(ObjectLayer& layer, const usize n)
{
  layer.objects.reserve(n);
}

auto AddObject(ObjectLayer& layer) -> Object&
{
  return layer.objects.emplace_back();
}

auto GetObjectCount(const ObjectLayer& layer) -> usize
{
  return layer.objects.size();
}

auto GetObject(const ObjectLayer& layer, const usize index) -> const Object&
{
  return layer.objects.at(index);
}

void ReserveLayers(GroupLayer& layer, const usize n)
{
  layer.layers.reserve(n);
}

auto AddLayer(GroupLayer& layer) -> Layer&
{
  return *layer.layers.emplace_back(std::make_unique<Layer>());
}

auto GetLayerCount(const GroupLayer& layer) -> usize
{
  return layer.layers.size();
}

auto GetLayer(const GroupLayer& layer, const usize index) -> const Layer&
{
  return *layer.layers.at(index);
}

void SetName(Property& property, const CStr name)
{
  assert(name);
  property.name = name;
}

void AssignInt(Property& property, const int32 value)
{
  property.value.SetValue(value);
}

void AssignFloat(Property& property, const float value)
{
  property.value.SetValue(value);
}

void AssignString(Property& property, const CStr str)
{
  assert(str);
  property.value.SetValue(std::string{str});
}

void AssignBool(Property& property, const bool value)
{
  property.value.SetValue(value);
}

void AssignFile(Property& property, const CStr path)
{
  assert(path);
  property.value.SetValue(std::filesystem::path{path});
}

void AssignFile(Property& property, const CPathStr path)
{
  assert(path);
  property.value.SetValue(std::filesystem::path{path});
}

void AssignObject(Property& property, const int32 id)
{
  property.value.SetValue(ObjectRef{id});
}

void AssignColor(Property& property, const Color& color)
{
  property.value.SetValue(cen::color{color.red, color.green, color.blue, color.alpha});
}

auto GetName(const Property& property) -> CStr
{
  return property.name.c_str();
}

auto GetType(const Property& property) -> PropertyType
{
  return property.value.GetType().value();
}

auto GetInt(const Property& property) -> int32
{
  return property.value.AsInt();
}

auto GetFloat(const Property& property) -> float
{
  return property.value.AsFloat();
}

auto GetString(const Property& property) -> CStr
{
  return property.value.AsString().c_str();
}

auto GetBool(const Property& property) -> bool
{
  return property.value.AsBool();
}

auto GetFile(const Property& property) -> CPathStr
{
  return property.value.AsFile().c_str();
}

auto GetObject(const Property& property) -> int32
{
  return property.value.AsObject().get();
}

auto GetColor(const Property& property) -> Color
{
  const auto& color = property.value.AsColor();
  return {color.red(), color.green(), color.blue(), color.alpha()};
}

}  // namespace Tactile::IO