#pragma once

#include <filesystem>     // path
#include <unordered_map>  // unordered_map

#include <tactile_def.hpp>

#include <centurion.hpp>  // irect
#include <entt/entt.hpp>  // entity, null

#include "core/region.hpp"

namespace Tactile {

struct TilesetContext final
{
  TilesetID next_id{};    ///< Next available tileset ID.
  TileID next_tile_id{};  ///< Next available global tile ID.
};

struct Tileset final
{
  TilesetID id{};        ///< Unique ID associated with the tileset.
  TileID first_id{};     ///< First associated global tile ID.
  TileID last_id{};      ///< Last associated global tile ID (inclusive).
  int32 tile_width{};    ///< Width of tiles in the tileset.
  int32 tile_height{};   ///< Height of tiles in the tileset.
  int32 tile_count{};    ///< Number of tiles associated with the tileset.
  int32 row_count{};     ///< Amount of tile rows.
  int32 column_count{};  ///< Amount of tile columns.
};

struct TilesetCache final
{
  std::unordered_map<TileID, cen::irect> source_rects;  ///< Tileset source rectangles.
  std::unordered_map<TileID, entt::entity> tiles;  ///< Tiles to `FancyTile` entities.
};

struct TilesetSelection final
{
  Maybe<Region> region;
};

struct ActiveTileset final
{
  entt::entity entity{entt::null};
};

}  // namespace Tactile
