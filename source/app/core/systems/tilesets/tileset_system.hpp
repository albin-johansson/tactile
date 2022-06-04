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

#include <centurion/fwd.hpp>
#include <entt/fwd.hpp>

#include "core/common/identifiers.hpp"
#include "core/common/ints.hpp"
#include "core/common/maybe.hpp"
#include "core/common/uuid.hpp"
#include "core/components/texture.hpp"
#include "core/region.hpp"
#include "core/systems/snapshot.hpp"
#include "core/tile_pos.hpp"

namespace tactile::sys {

/**
 * \ingroup systems
 * \defgroup tileset-system Tileset System
 *
 * Manages available tilesets.
 *
 * There are two kinds of tilesets that are handled in slightly different ways.
 * First, there are tileset documents, which have their own registries. These documents
 * may exist at runtime even if no maps are using them. The second type of tilesets are
 * _tileset references_, these are stored in map registries and reference a loaded tileset
 * document.
 */

/// \addtogroup tileset-system
/// \{

/**
 * Updates the state of all tilesets in a map.
 *
 * This function should be each frame, as it handles aspects related to tile caching.
 *
 * \param mapRegistry a map document registry.
 */
void update_tilesets(entt::registry& mapRegistry);

/**
 * Selects the tileset associated with the specified ID.
 *
 * \pre `id` must be associated with an existing tileset.
 *
 * \param mapRegistry a map document registry.
 * \param id the ID of the tileset to select.
 */
void select_tileset(entt::registry& mapRegistry, const UUID& id);

/**
 * Attaches a tileset to a map document.
 *
 * \param mapRegistry a map document registry.
 * \param tilesetId the ID of the external tileset document.
 * \param tileset the basic tileset information.
 * \param firstTile optional first tile identifier for the tileset.
 */
void attach_tileset(entt::registry& mapRegistry,
                    const UUID& tilesetId,
                    const comp::Tileset& tileset,
                    Maybe<TileID> firstTile = nothing);

/**
 * Detaches a tileset from a map document.
 *
 * \param mapRegistry a map document registry.
 * \param tilesetId the ID of the external tileset document.
 *
 * \return a snapshot of the removed tileset.
 */
auto detach_tileset(entt::registry& mapRegistry, const UUID& tilesetId)
    -> TilesetSnapshot;

/**
 * Sets the region of the active tileset that is selected.
 *
 * \pre There must be an active tileset when this function is invoked.
 *
 * \param mapRegistry the registry that will be modified.
 * \param region the region of the tileset that will be selected.
 */
void update_tileset_selection(entt::registry& mapRegistry, const Region& region);

/**
 * Attempts to find a tileset with a specific ID.
 *
 * \param mapRegistry the associated registry.
 * \param id the ID associated with the desired tileset.
 *
 * \return the associated tileset entity; a null entity is returned if there is none.
 */
[[nodiscard]] auto find_tileset(const entt::registry& mapRegistry, const UUID& id)
    -> entt::entity;

[[nodiscard]] auto find_tile(const entt::registry& registry, TileID id) -> entt::entity;

[[nodiscard]] auto get_tile_entity(const entt::registry& registry, TileID id)
    -> entt::entity;

/**
 * Attempts to find the tileset that contains a specific tile.
 *
 * \param registry the associated registry.
 * \param id the tile ID located in the tileset to find.
 *
 * \return the found tileset entity; a null entity is returned otherwise.
 */
[[nodiscard]] auto find_tileset_with_tile(const entt::registry& registry, TileID id)
    -> entt::entity;

/**
 * Returns the active tileset entity, if there is one.
 *
 * \param registry the document registry.
 *
 * \return the active tileset entity; a null entity is returned if there is none.
 */
[[nodiscard]] auto find_active_tileset(const entt::registry& registry) -> entt::entity;

/**
 * Indicates whether or not the active tileset has a non-empty tile selection.
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
 * Indicates whether or not the active tileset has a _single_ tile selected.
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
 * Returns the rendered appearance of a tile.
 *
 * This function is used to determine the correct tile to render for animated
 * tiles. In other words, this function will simply return the supplied tile for
 * non-animated tiles.
 *
 * \param mapRegistry the document registry.
 * \param tilesetEntity the parent tileset entity.
 * \param id the tile to query.
 *
 * \return the tile that should be rendered when the specified tile is encountered.
 */
[[nodiscard]] auto get_tile_to_render(const entt::registry& mapRegistry,
                                      entt::entity tilesetEntity,
                                      TileID id) -> TileID;

/**
 * Returns the region out of the tileset texture that should be rendered when
 * rendering the specified tile.
 *
 * \note You should use the identifier returned by `get_tile_to_render()` when calling
 * this function.
 *
 * \param mapRegistry the associated registry.
 * \param tilesetEntity the parent tileset entity.
 * \param id the identifier for the tile that will be queried.
 *
 * \return the region of the tileset texture that should be rendered for the tile.
 *
 * \see `get_tile_to_render()`
 */
[[nodiscard]] auto get_source_rect(const entt::registry& mapRegistry,
                                   entt::entity tilesetEntity,
                                   TileID id) -> const cen::irect&;

/**
 * Converts a global tile identifier to its local counterpart.
 *
 * A "local" tile identifier is a just a tile index in the parent tileset.
 *
 * \param mapRegistry the associated registry.
 * \param global a global tile identifier that will be converted.
 *
 * \return the corresponding local tile identifier;
 *         an empty optional if something went wrong.
 */
[[nodiscard]] auto convert_to_local(const entt::registry& mapRegistry, TileID global)
    -> Maybe<TileID>;

/// \} End of group tileset-system

}  // namespace tactile::sys
