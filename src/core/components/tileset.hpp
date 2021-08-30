#pragma once

#include <centurion.hpp>  // irect
#include <entt.hpp>       // entity, null
#include <filesystem>     // path

#include "aliases/col.hpp"
#include "aliases/hash_map.hpp"
#include "aliases/maybe.hpp"
#include "aliases/row.hpp"
#include "aliases/tile_id.hpp"
#include "aliases/tileset_id.hpp"
#include "core/region.hpp"

namespace Tactile {

struct Tileset final
{
  tileset_id id;       ///< Unique ID associated with the tileset.
  tile_id first_id;    ///< First associated global tile ID.
  tile_id last_id;     ///< Last associated global tile ID (inclusive).
  int tile_width{};    ///< Width of tiles in the tileset.
  int tile_height{};   ///< Height of tiles in the tileset.
  int tile_count{};    ///< Number of tiles associated with the tileset.
  row_t row_count;     ///< Amount of tile rows.
  col_t column_count;  ///< Amount of tile columns.
  std::filesystem::path image_path;  ///< File path of tileset image.
};

struct TilesetCache final
{
  HashMap<tile_id, cen::irect> source_rects;  ///< Tileset source rectangles.
  HashMap<tile_id, entt::entity> tiles;  ///< Maps tiles to `FancyTile` entities.
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
