#pragma once

#include <filesystem>     // path
#include <unordered_map>  // unordered_map

#include <centurion.hpp>
#include <entt/entt.hpp>
#include <tactile_def.hpp>

#include "core/region.hpp"

namespace tactile {

/**
 * \brief Tileset state for a single map.
 */
struct TilesetContext final
{
  TilesetID next_id{};    ///< Next available tileset ID.
  TileID next_tile_id{};  ///< Next available global tile ID.

  /**
   * \brief Maps all global tile identifiers to the associated tileset.
   */
  std::unordered_map<TileID, entt::entity> tile_to_tileset;
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

/**
 * \brief Cached information about a single tileset.
 */
struct TilesetCache final
{
  std::unordered_map<TileID, cen::irect> source_rects;  ///< Tileset source rectangles.
  std::unordered_map<TileID, entt::entity> tiles;       ///< Additional tile info.

  /**
   * \brief Frame-by-frame cache that maps tiles to the tile that should be rendered.
   */
  mutable std::unordered_map<TileID, TileID> source_to_render;
};

struct TilesetSelection final
{
  Maybe<Region> region;
};

struct ActiveTileset final
{
  entt::entity entity{entt::null};
};

}  // namespace tactile
