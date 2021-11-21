#pragma once

#include <tactile_def.hpp>

#include <centurion.hpp>  // irect
#include <entt/entt.hpp>  // registry, entity

#include "core/components/texture.hpp"
#include "core/map_position.hpp"
#include "core/region.hpp"
#include "tileset_snapshot.hpp"

namespace Tactile::Sys {

/// \name Tileset system
/// \{

/**
 * \brief Creates a tileset entity.
 *
 * \details The created entity will feature the following components:
 * - `Tileset`
 * - `Texture`
 * - `TilesetCache`
 * - `TilesetSelection`
 * - `UvTileSize`
 * - `PropertyContext`
 *
 * \param registry the associated registry.
 * \param firstId the first global tile ID associated with the tileset.
 * \param texture information about the associated texture.
 * \param tileWidth the width of tiles in the tileset.
 * \param tileHeight the height of tiles in the tileset.
 *
 * \return the created tileset entity.
 *
 * \since 0.2.0
 */
auto MakeTileset(entt::registry& registry,
                 TileID firstId,
                 const Texture& texture,
                 int32 tileWidth,
                 int32 tileHeight) -> entt::entity;

// This overload should be used when the user adds new tilesets (i.e. not from parsing)
auto MakeTileset(entt::registry& registry,
                 const Texture& texture,
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
 * \see CopyTileset()
 */
auto RestoreTileset(entt::registry& registry, TilesetSnapshot snapshot) -> entt::entity;

/**
 * \brief Creates a snapshot of a tileset.
 *
 * \param registry the registry that hosts the tileset.
 * \param source the source tileset entity to create a snapshot of.
 *
 * \return a snapshot of the tileset.
 *
 * \see RestoreTileset()
 */
[[nodiscard]] auto CopyTileset(const entt::registry& registry, entt::entity source)
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
void SelectTileset(entt::registry& registry, TilesetID id);

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
void RemoveTileset(entt::registry& registry, TilesetID id);

/**
 * \brief Sets the region of the active tileset that is selected.
 *
 * \pre There must be an active tileset when this function is invoked.
 *
 * \param registry the registry that will be modified.
 * \param region the region of the tileset that will be selected.
 */
void UpdateTilesetSelection(entt::registry& registry, const Region& region);

/**
 * \brief Attempts to find the tileset associated with the specified ID.
 *
 * \param registry the associated registry.
 * \param id the ID associated with the desired tileset.
 *
 * \return the associated tileset entity; a null entity is returned if there is none.
 */
[[nodiscard]] auto FindTileset(const entt::registry& registry, TilesetID id)
    -> entt::entity;

/**
 * \brief Attempts to find the tileset that contains a specific tile.
 *
 * \param registry the associated registry.
 * \param id the tile ID located in the tileset to find.
 *
 * \return the found tileset entity; a null entity is returned otherwise.
 */
[[nodiscard]] auto FindTilesetWithTile(const entt::registry& registry, TileID id)
    -> entt::entity;

/**
 * \brief Returns the active tileset entity, if there is one.
 *
 * \param registry the registry that will be queried.
 *
 * \return the active tileset entity; the null entity is returned if there is none.
 */
[[nodiscard]] auto GetActiveTileset(const entt::registry& registry) -> entt::entity;

/**
 * \brief Indicates whether or not the active tileset has a non-empty tile selection.
 *
 * \param registry the registry that will be queried.
 *
 * \return `true` if there is an active tileset with a non-empty selection; `false`
 * otherwise.
 *
 * \see IsSingleTileSelectedInTileset()
 */
[[nodiscard]] auto HasNonEmptyTilesetSelection(const entt::registry& registry) -> bool;

/**
 * \brief Indicates whether or not the active tileset has a _single_ tile selected.
 *
 * \param registry the registry that will be queried.
 *
 * \return `true` if there is an active tileset with a single tile selected; `false`
 * otherwise.
 *
 * \see HasNonEmptyTilesetSelection()
 */
[[nodiscard]] auto IsSingleTileSelectedInTileset(const entt::registry& registry) -> bool;

/**
 * \brief Returns the ID of the tile that should be rendered when the specified tile is
 * encountered.
 *
 * \details This function is used to determine the correct tile to render with animated
 * tiles. In other words, this function will simply return the supplied ID for
 * non-animated tiles.
 *
 * \param registry the associated registry.
 * \param tilesetEntity the parent tileset entity.
 * \param id the identifier for the tile that will be queried.
 *
 * \return the ID of the tile that should be rendered when the specified tile is
 * encountered.
 */
[[nodiscard]] auto GetTileToRender(const entt::registry& registry,
                                   entt::entity tilesetEntity,
                                   TileID id) -> TileID;

/**
 * \brief Returns the region out of the tileset texture that should be rendered when
 * rendering the specified tile.
 *
 * \note You should use the identifier returned by `GetTileToRender()` when calling this
 * function.
 *
 * \param registry the associated registry.
 * \param tilesetEntity the parent tileset entity.
 * \param id the identifier for the tile that will be queried.
 *
 * \return the region of the tileset texture that should be rendered for the tile.
 *
 * \see `GetTileToRender()`
 */
[[nodiscard]] auto GetSourceRect(const entt::registry& registry,
                                 entt::entity tilesetEntity,
                                 TileID id) -> const cen::irect&;

/**
 * \brief Returns the identifier of a tile at a certain position in a tileset.
 *
 * \param registry the registry that will be queried.
 * \param entity the tileset entity.
 * \param position the position of the tile in the tileset.
 *
 * \return the identifier of the found tile; the empty tile is returned otherwise.
 */
[[nodiscard]] auto GetTileFromTileset(const entt::registry& registry,
                                      entt::entity entity,
                                      const MapPosition& position) -> TileID;

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
[[nodiscard]] auto ConvertToLocal(const entt::registry& registry, TileID global)
    -> Maybe<TileID>;

/// \} End of tileset system

}  // namespace Tactile::Sys
