// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <cstdint>        // int32_t
#include <unordered_map>  // unordered_map
#include <vector>  // vector

#include "tactile/base/id.hpp"
#include "tactile/base/numeric/vec.hpp"
#include "tactile/base/numeric/extent_2d.hpp"
#include "tactile/core/entity/entity.hpp"
#include "tactile/core/io/texture.hpp"

namespace tactile::core {

/**
 * Represents an sequence of tile identifiers.
 */
struct TileRange final
{
  /** The first associated tile identifier. */
  TileID first_id;

  /** The number of tile identifiers (starting at \c first_id). */
  std::int32_t count;
};

/**
 * Represents the information needed to construct a tileset.
 */
struct TilesetSpec final
{
  /** The logical tile size. */
  Int2 tile_size;

  /** The associated texture. */
  CTexture texture;
};

/**
 * Context component used to map tile identifiers to tilesets.
 */
struct CTileCache final
{
  /** Maps tile identifiers to the associated tilesets. */
  std::unordered_map<TileID, EntityID> tileset_mapping;
};

/**
 * A component that represents a tileset definition.
 */
struct CTileset final
{
  /** The logical size of all tiles. */
  Int2 tile_size;

  /** The size of all tiles in texture coordinates. */
  Float2 uv_tile_size;

  /** The size of the tileset. */
  Extent2D extent;

  /** Tracks the associated tiles. Indexed using \c TileIndex values. */
  std::vector<EntityID> tiles;
};

/**
 * A component that provides information about tilesets attached to maps.
 *
 * \note
 * This component shouldn't be used by tilesets in tileset documents.
 */
struct CTilesetInstance final
{
  /** The associated global tile identifiers. */
  TileRange tile_range;

  /** Indicates whether the tileset is embedded in the map file when saved. */
  bool is_embedded;
};

}  // namespace tactile::core
