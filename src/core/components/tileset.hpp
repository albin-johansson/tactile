#pragma once

#include <centurion.hpp>  // irect
#include <entt.hpp>       // entity, null
#include <filesystem>     // path

#include "aliases/hash_map.hpp"
#include "aliases/ints.hpp"
#include "aliases/maybe.hpp"
#include "aliases/tile_id.hpp"
#include "aliases/tileset_id.hpp"
#include "core/region.hpp"

namespace Tactile {

struct TilesetContext final
{
  TilesetID next_id;    ///< Next available tileset ID.
  TileID next_tile_id;  ///< Next available global tile ID.
};

struct Tileset final
{
  TilesetID id;          ///< Unique ID associated with the tileset.
  TileID first_id;       ///< First associated global tile ID.
  TileID last_id;        ///< Last associated global tile ID (inclusive).
  int tile_width{};      ///< Width of tiles in the tileset.
  int tile_height{};     ///< Height of tiles in the tileset.
  int tile_count{};      ///< Number of tiles associated with the tileset.
  int32 row_count{};     ///< Amount of tile rows.
  int32 column_count{};  ///< Amount of tile columns.
};

struct TilesetCache final
{
  HashMap<TileID, cen::irect> source_rects;  ///< Tileset source rectangles.
  HashMap<TileID, entt::entity> tiles;       ///< Maps tiles to `FancyTile` entities.
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
