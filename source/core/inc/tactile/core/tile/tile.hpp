// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/vector.hpp"
#include "tactile/base/id.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/entity/entity.hpp"

namespace tactile {

class Registry;

/// \addtogroup tile
/// \{

/**
 * A component that represents a tile definition.
 */
struct CTile final
{
  /** The local identifier of the tile. */
  TileIndex index;

  /** The embedded objects, if any. */
  Vector<EntityID> objects;
};

/**
 * Indicates whether an entity is a tile.
 *
 * \details
 * Tile entities feature the following components. \n
 * - \c CMeta \n
 * - \c CTile
 *
 * \param registry The associated registry.
 * \param entity   The entity to check.
 *
 * \return
 * True if the entity is a tile; false otherwise.
 */
[[nodiscard]]
auto is_tile(const Registry& registry, EntityID entity) -> bool;

/**
 * Creates an empty tile.
 *
 * \param registry The associated registry.
 * \param index    The index of the tile.
 *
 * \return
 * A tile entity.
 */
[[nodiscard]]
auto make_tile(Registry& registry, TileIndex index) -> EntityID;

/**
 * Destroys a tile.
 *
 * \note
 * This function will also destroy any objects embedded in the tile.
 *
 * \param registry    The associated registry.
 * \param tile_entity The target tile entity.
 */
void destroy_tile(Registry& registry, EntityID tile_entity);

/// \}

}  // namespace tactile
