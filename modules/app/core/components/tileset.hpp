#pragma once

#include <filesystem>     // path
#include <unordered_map>  // unordered_map

#include <centurion.hpp>
#include <entt/entt.hpp>

#include "core/region.hpp"
#include "tactile_def.hpp"

namespace tactile::comp {

/**
 * \brief Tileset state for a single map.
 */
struct tileset_context final
{
  tileset_id next_id{};    ///< Next available tileset ID.
  tile_id next_tile_id{};  ///< Next available global tile ID.

  /**
   * \brief Maps all global tile identifiers to the associated tileset.
   */
  std::unordered_map<tile_id, entt::entity> tile_to_tileset;
};

struct tileset final
{
  tileset_id id{};       ///< Unique ID associated with the tileset.
  tile_id first_id{};    ///< First associated global tile ID.
  tile_id last_id{};     ///< Last associated global tile ID (inclusive).
  int32 tile_width{};    ///< Width of tiles in the tileset.
  int32 tile_height{};   ///< Height of tiles in the tileset.
  int32 tile_count{};    ///< Number of tiles associated with the tileset.
  int32 row_count{};     ///< Amount of tile rows.
  int32 column_count{};  ///< Amount of tile columns.
};

/**
 * \brief Cached information about a single tileset.
 */
struct tileset_cache final
{
  std::unordered_map<tile_id, cen::irect> source_rects;  ///< Tileset source rectangles.
  std::unordered_map<tile_id, entt::entity> tiles;       ///< Additional tile info.

  /**
   * \brief Frame-by-frame cache that maps tiles to the tile that should be rendered.
   */
  mutable std::unordered_map<tile_id, tile_id> source_to_render;
};

struct tileset_selection final
{
  maybe<Region> region;
};

struct active_tileset final
{
  entt::entity entity{entt::null};
};

}  // namespace tactile::comp
