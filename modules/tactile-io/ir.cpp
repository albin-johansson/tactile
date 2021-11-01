#include "ir.hpp"

#include <cassert>     // assert
#include <filesystem>  // path, absolute
#include <string>      // string
#include <variant>     // variant, get, holds_alternative
#include <vector>      // vector

#include <tactile-base/property_value.hpp>

#include "ir_definitions.hpp"

namespace Tactile::IO {

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

auto IsTileLayer(const Layer& layer) -> bool
{
  return std::holds_alternative<TileLayer>(layer.data);
}

auto IsObjectLayer(const Layer& layer) -> bool
{
  return std::holds_alternative<ObjectLayer>(layer.data);
}

auto IsGroupLayer(const Layer& layer) -> bool
{
  return std::holds_alternative<GroupLayer>(layer.data);
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