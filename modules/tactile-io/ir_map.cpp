#include "ir_map.hpp"

#include <cassert>     // assert
#include <filesystem>  // absolute

#include "ir_definitions.hpp"

namespace Tactile::IO {

auto NewMap() -> Map*
{
  return new Map{};
}

void DeleteMap(Map* map) noexcept
{
  delete map;
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

auto GetPath(const Map& map) -> CPathStr
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

auto GetTileset(Map& map, const usize index) -> Tileset&
{
  return map.tilesets.at(index);
}

auto GetTileset(const Map& map, const usize index) -> const Tileset&
{
  return map.tilesets.at(index);
}

auto GetLayer(Map& map, const usize index) -> Layer&
{
  return map.layers.at(index);
}

auto GetLayer(const Map& map, const usize index) -> const Layer&
{
  return map.layers.at(index);
}

auto GetProperty(Map& map, const usize index) -> Property&
{
  return map.properties.at(index);
}

auto GetProperty(const Map& map, const usize index) -> const Property&
{
  return map.properties.at(index);
}

}  // namespace Tactile::IO