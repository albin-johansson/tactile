/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <centurion.hpp>
#include <entt/entt.hpp>

#include "core/components/texture.hpp"
#include "core/region.hpp"
#include "core/tile_position.hpp"
#include "snapshot.hpp"
#include "tactile.hpp"

namespace tactile::sys {

/// \name Tileset system
/// \{

/**
 * \brief Updates the state of tilesets.
 *
 * \details This function should be each frame, as it handles aspects related to tile
 * caching.
 *
 * \param registry the document registry.
 */
void update_tilesets(entt::registry& registry);

/**
 * \brief Creates a tileset entity.
 *
 * \details The created entity will feature the following components:
 * - `AttributeContext`
 * - `Tileset`
 * - `TilesetCache`
 * - `TilesetSelection`
 * - `Texture`
 * - `UvTileSize`
 * - `Viewport`
 *
 * \param registry the document registry.
 * \param firstId the first global tile ID associated with the tileset.
 * \param texture information about the associated texture.
 * \param tileWidth the width of tiles in the tileset.
 * \param tileHeight the height of tiles in the tileset.
 *
 * \return the created tileset entity.
 */
auto make_tileset(entt::registry& registry,
                  tile_id firstId,
                  const comp::Texture& texture,
                  int32 tileWidth,
                  int32 tileHeight) -> entt::entity;

// This overload should be used when the user adds new tilesets (i.e. not from parsing)
auto make_tileset(entt::registry& registry,
                  const comp::Texture& texture,
                  int32 tileWidth,
                  int32 tileHeight) -> entt::entity;

/**
 * \brief Restores a tileset from a snapshot.
 *
 * \param registry the registry that will host the restored tileset.
 * \param snapshot the snapshot of the tileset that will be restored.
 *
 * \return the entity identifier of the restored tileset.
 *
 * \see copy_tileset()
 */
auto restore_tileset(entt::registry& registry, TilesetSnapshot snapshot) -> entt::entity;

/**
 * \brief Creates a snapshot of a tileset.
 *
 * \param registry the registry that hosts the tileset.
 * \param source the source tileset entity to create a snapshot of.
 *
 * \return a snapshot of the tileset.
 *
 * \see restore_tileset()
 */
[[nodiscard]] auto copy_tileset(const entt::registry& registry, entt::entity source)
    -> TilesetSnapshot;

/**
 * \brief Selects the tileset associated with the specified ID.
 *
 * \pre `id` must be associated with an existing tileset.
 *
 * \param registry the associated registry.
 *
 * \param id the ID associated with the tileset that will be selected.
 */
void select_tileset(entt::registry& registry, tileset_id id);

/**
 * \brief Removes the tileset associated with the specified ID.
 *
 * \pre `id` must be associated with an existing tileset.
 *
 * \details The active tileset is either reset or changed to another available tileset if
 * the specified tileset is active at the time of invocation.
 *
 * \param registry the associated registry.
 *
 * \param id the ID associated with the tileset that will be removed.
 */
void remove_tileset(entt::registry& registry, tileset_id id);

/**
 * \brief Sets the region of the active tileset that is selected.
 *
 * \pre There must be an active tileset when this function is invoked.
 *
 * \param registry the registry that will be modified.
 * \param region the region of the tileset that will be selected.
 */
void update_tileset_selection(entt::registry& registry, const Region& region);

/**
 * \brief Attempts to find the tileset associated with the specified ID.
 *
 * \param registry the associated registry.
 * \param id the ID associated with the desired tileset.
 *
 * \return the associated tileset entity; a null entity is returned if there is none.
 */
[[nodiscard]] auto find_tileset(const entt::registry& registry, tileset_id id)
    -> entt::entity;

[[nodiscard]] auto get_tileset_entity(const entt::registry& registry, tileset_id id)
    -> entt::entity;

[[nodiscard]] auto find_tile(const entt::registry& registry, tile_id id) -> entt::entity;

[[nodiscard]] auto get_tile_entity(const entt::registry& registry, tile_id id)
    -> entt::entity;

/**
 * \brief Attempts to find the tileset that contains a specific tile.
 *
 * \param registry the associated registry.
 * \param id the tile ID located in the tileset to find.
 *
 * \return the found tileset entity; a null entity is returned otherwise.
 */
[[nodiscard]] auto find_tileset_with_tile(const entt::registry& registry, tile_id id)
    -> entt::entity;

/**
 * \brief Returns the active tileset entity, if there is one.
 *
 * \param registry the document registry.
 *
 * \return the active tileset entity; a null entity is returned if there is none.
 */
[[nodiscard]] auto find_active_tileset(const entt::registry& registry) -> entt::entity;

/**
 * \brief Indicates whether or not the active tileset has a non-empty tile selection.
 *
 * \param registry the registry that will be queried.
 *
 * \return `true` if there is an active tileset with a non-empty selection; `false`
 * otherwise.
 *
 * \see is_single_tile_selected_in_tileset()
 */
[[nodiscard]] auto is_tileset_selection_not_empty(const entt::registry& registry) -> bool;

/**
 * \brief Indicates whether or not the active tileset has a _single_ tile selected.
 *
 * \param registry the registry that will be queried.
 *
 * \return `true` if there is an active tileset with a single tile selected; `false`
 * otherwise.
 *
 * \see is_tileset_selection_not_empty()
 */
[[nodiscard]] auto is_single_tile_selected_in_tileset(const entt::registry& registry)
    -> bool;

/**
 * \brief Returns the rendered appearance of a tile.
 *
 * \details This function is used to determine the correct tile to render for animated
 * tiles. In other words, this function will simply return the supplied tile for
 * non-animated tiles.
 *
 * \param registry the document registry.
 * \param tilesetEntity the parent tileset entity.
 * \param id the tile to query.
 *
 * \return the tile that should be rendered when the specified tile is encountered.
 */
[[nodiscard]] auto get_tile_to_render(const entt::registry& registry,
                                      entt::entity tilesetEntity,
                                      tile_id id) -> tile_id;

/**
 * \brief Returns the region out of the tileset texture that should be rendered when
 * rendering the specified tile.
 *
 * \note You should use the identifier returned by `get_tile_to_render()` when calling
 * this function.
 *
 * \param registry the associated registry.
 * \param tilesetEntity the parent tileset entity.
 * \param id the identifier for the tile that will be queried.
 *
 * \return the region of the tileset texture that should be rendered for the tile.
 *
 * \see `get_tile_to_render()`
 */
[[nodiscard]] auto get_source_rect(const entt::registry& registry,
                                   entt::entity tilesetEntity,
                                   tile_id id) -> const cen::irect&;

/**
 * \brief Returns the identifier of a tile at a certain position in a tileset.
 *
 * \param registry the registry that will be queried.
 * \param entity the tileset entity.
 * \param position the position of the tile in the tileset.
 *
 * \return the identifier of the found tile; the empty tile is returned otherwise.
 */
[[nodiscard]] auto get_tile_from_tileset(const entt::registry& registry,
                                         entt::entity entity,
                                         const tile_position& position) -> tile_id;

/**
 * \brief Converts a global tile identifier to its local counterpart.
 *
 * \details A "local" tile identifier is a basically a tile index in the parent
 * tileset.
 *
 * \param registry the associated registry.
 * \param global a global tile identifier that will be converted.
 *
 * \return the corresponding local tile identifier; `nothing` if something went wrong.
 */
[[nodiscard]] auto convert_to_local(const entt::registry& registry, tile_id global)
    -> Maybe<tile_id>;

/// \} End of tileset system

}  // namespace tactile::sys
