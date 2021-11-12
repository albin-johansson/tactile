#include "ir_layer.hpp"

#include <algorithm>  // clamp
#include <cassert>    // assert
#include <memory>     // make_unique
#include <variant>    // get, get_if, holds_alternative

#include <tactile_stdlib.hpp>

#include "ir_definitions.hpp"

namespace Tactile::IO {

auto MarkAsTileLayer(Layer& layer) -> TileLayer&
{
  layer.type = LayerType::TileLayer;
  return layer.data.emplace<TileLayer>();
}

auto MarkAsObjectLayer(Layer& layer) -> ObjectLayer&
{
  layer.type = LayerType::ObjectLayer;
  return layer.data.emplace<ObjectLayer>();
}

auto MarkAsGroupLayer(Layer& layer) -> GroupLayer&
{
  layer.type = LayerType::GroupLayer;
  return layer.data.emplace<GroupLayer>();
}

auto GetTileLayer(Layer& layer) -> TileLayer&
{
  if (auto* ptr = std::get_if<TileLayer>(&layer.data)) {
    return *ptr;
  }
  else {
    throw TactileError{"Layer was not a tile layer!"};
  }
}

auto GetTileLayer(const Layer& layer) -> const TileLayer&
{
  if (const auto* ptr = std::get_if<TileLayer>(&layer.data)) {
    return *ptr;
  }
  else {
    throw TactileError{"Layer was not a tile layer!"};
  }
}

auto GetObjectLayer(Layer& layer) -> ObjectLayer&
{
  if (auto* ptr = std::get_if<ObjectLayer>(&layer.data)) {
    return *ptr;
  }
  else {
    throw TactileError{"Layer was not an object layer!"};
  }
}

auto GetObjectLayer(const Layer& layer) -> const ObjectLayer&
{
  if (const auto* ptr = std::get_if<ObjectLayer>(&layer.data)) {
    return *ptr;
  }
  else {
    throw TactileError{"Layer was not an object layer!"};
  }
}

auto GetGroupLayer(Layer& layer) -> GroupLayer&
{
  if (auto* ptr = std::get_if<GroupLayer>(&layer.data)) {
    return *ptr;
  }
  else {
    throw TactileError{"Layer was not a group layer!"};
  }
}

auto GetGroupLayer(const Layer& layer) -> const GroupLayer&
{
  if (const auto* ptr = std::get_if<GroupLayer>(&layer.data)) {
    return *ptr;
  }
  else {
    throw TactileError{"Layer was not a group layer!"};
  }
}

void SetId(Layer& layer, const int32 id)
{
  layer.id = id;
}

void SetIndex(Layer& layer, const usize index)
{
  layer.index = index;
}

void SetName(Layer& layer, const CStr name)
{
  assert(name);
  layer.name = name;
}

void SetOpacity(Layer& layer, const float opacity)
{
  layer.opacity = std::clamp(opacity, 0.0f, 1.0f);
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

auto GetType(const Layer& layer) -> LayerType
{
  return layer.type;
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

auto GetProperty(Layer& layer, const usize index) -> Property&
{
  return layer.properties.at(index);
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
  layer.tiles.at(row).at(column) = tile;
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
  return layer.tiles.at(row).at(column);
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