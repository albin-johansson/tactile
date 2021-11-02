#include "ir.hpp"

#include <cassert>     // assert
#include <variant>     // variant, get, holds_alternative

#include <tactile-base/tactile_std.hpp>

#include "ir_definitions.hpp"

namespace Tactile::IO {

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

void ReserveTiles(TileLayer& layer, const usize nRows, const usize nCols)
{
  layer.row_count = nRows;
  layer.col_count = nCols;
  layer.tiles = MakeTileMatrix(nRows, nCols);
}

void SetTile(TileLayer& layer, const usize row, const usize column, const int32 tile)
{
  layer.tiles.at(row).at(column) = TileID{tile};
}

auto GetRowCount(const TileLayer& layer) -> usize
{
  return layer.row_count;
}

auto GetColumnCount(const TileLayer& layer) -> usize
{
  return layer.col_count;
}

auto GetTile(const TileLayer& layer, const usize row, const usize column) -> int32
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

}  // namespace Tactile::IO