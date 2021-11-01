#include "ir_tileset.hpp"

#include <cassert>     // assert
#include <filesystem>  // absolute

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

auto GetTileInfo(Tileset& tileset, const usize index) -> Tile&
{
  return tileset.tiles.at(index);
}

auto GetTileInfo(const Tileset& tileset, const usize index) -> const Tile&
{
  return tileset.tiles.at(index);
}

auto GetPropertyCount(const Tileset& tileset) -> usize
{
  return tileset.properties.size();
}

auto GetProperty(Tileset& tileset, usize index) -> Property&
{
  return tileset.properties.at(index);
}

auto GetProperty(const Tileset& tileset, const usize index) -> const Property&
{
  return tileset.properties.at(index);
}

}  // namespace Tactile::IO