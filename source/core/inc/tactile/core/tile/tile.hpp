// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <expected>      // expected
#include <system_error>  // error_code

#include "tactile/base/container/vector.hpp"
#include "tactile/base/id.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/entity/entity.hpp"

namespace tactile {

class Registry;

namespace ir {
struct Tile;
}  // namespace ir

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
 * Creates a tile from an intermediate representation.
 *
 * \param registry The associated registry.
 * \param tile     The intermediate tile representation.
 *
 * \return
 * A tile entity identifier if successful; an error code otherwise.
 */
[[nodiscard]]
auto make_tile(Registry& registry,
               const ir::Tile& ir_tile) -> std::expected<EntityID, std::error_code>;

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

/**
 * Creates a deep copy of a tile.
 *
 * \pre The specified entity must be a tile.
 *
 * \param registry    The associated registry.
 * \param tile_entity The tile that will be copied.
 *
 * \return
 * A tile entity.
 */
[[nodiscard]]
auto copy_tile(Registry& registry, EntityID tile_entity) -> EntityID;

/**
 * Indicates whether a tile is "plain".
 *
 * \pre The provided entity identifier must reference a tile.
 *
 * \details
 * A tile is considered plain if it features no animation, nested objects,
 * properties, nor components. This means that plain tiles don't need to be
 * included in saved tilesets.
 *
 * \param registry The associated registry.
 * \param tile_id  The identifier of the tile to check.
 *
 * \return
 * True if the tile is plain; false otherwise.
 */
[[nodiscard]]
auto is_tile_plain(const Registry& registry, EntityID tile_id) -> bool;

}  // namespace tactile
