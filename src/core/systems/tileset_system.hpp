#pragma once

#include <centurion.hpp>  // irect
#include <entt.hpp>       // registry, entity

#include "aliases/maybe.hpp"
#include "aliases/tile_id.hpp"
#include "aliases/tileset_id.hpp"
#include "core/map_position.hpp"
#include "core/region.hpp"
#include "tileset_snapshot.hpp"
#include "utils/texture_info.hpp"

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
 * \param id the ID that will be associated with the created tileset.
 * \param firstId the first global tile ID associated with the tileset.
 * \param info information about the associated texture.
 * \param tileWidth the width of tiles in the tileset.
 * \param tileHeight the height of tiles in the tileset.
 *
 * \return the created tileset entity.
 *
 * \since 0.2.0
 */
auto MakeTileset(entt::registry& registry,
                 TilesetID id,
                 TileID firstId,
                 const TextureInfo& info,
                 int tileWidth,
                 int tileHeight) -> entt::entity;

auto AddTileset(entt::registry& registry,
                const TextureInfo& info,
                int tileWidth,
                int tileHeight) -> entt::entity;

auto RestoreTileset(entt::registry& registry, TilesetSnapshot snapshot)
    -> entt::entity;

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
 *
 * \since 0.2.0
 */
void SelectTileset(entt::registry& registry, TilesetID id);

/**
 * \brief Removes the tileset associated with the specified ID.
 *
 * \pre `id` must be associated with an existing tileset.
 *
 * \details The active tileset is reset if the specified tileset is active at the
 * time of invocation.
 *
 * \param registry the associated registry.
 *
 * \param id the ID associated with the tileset that will be removed.
 *
 * \since 0.2.0
 */
void RemoveTileset(entt::registry& registry, TilesetID id);

void UpdateTilesetSelection(entt::registry& registry, const Region& region);

/**
 * \brief Attempts to find the tileset associated with the specified ID.
 *
 * \param registry the associated registry.
 * \param id the ID associated with the desired tileset.
 *
 * \return the associated tileset entity; a null entity is returned if there is none.
 *
 * \since 0.2.0
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
 *
 * \since 0.2.0
 */
[[nodiscard]] auto FindTileset(const entt::registry& registry, TileID id)
    -> entt::entity;

[[nodiscard]] auto GetActiveTileset(const entt::registry& registry) -> entt::entity;

[[nodiscard]] auto HasNonEmptyTilesetSelection(const entt::registry& registry)
    -> bool;

[[nodiscard]] auto IsSingleTileSelectedInTileset(const entt::registry& registry)
    -> bool;

/**
 * \brief Returns the ID of the tile that should be rendered when the specified tile
 * is encountered.
 *
 * \details This function is used to determine the correct tile to render with
 * animated tiles. In other words, this function will simply return the supplied ID
 * for non-animated tiles.
 *
 * \param registry the associated registry.
 * \param tilesetEntity the parent tileset entity.
 * \param id the identifier for the tile that will be queried.
 *
 * \return the ID of the tile that should be rendered when the specified tile is
 * encountered.
 *
 * \since 0.2.0
 */
[[nodiscard]] auto GetTileToRender(const entt::registry& registry,
                                   entt::entity tilesetEntity,
                                   TileID id) -> TileID;

/**
 * \brief Returns the region out of the tileset texture that should be rendered when
 * rendering the specified tile.
 *
 * \note You should use the identifier returned by `GetTileToRender()` when
 * calling this function.
 *
 * \param registry the associated registry.
 * \param tilesetEntity the parent tileset entity.
 * \param id the identifier for the tile that will be queried.
 *
 * \return the region of the tileset texture that should be rendered for the tile.
 *
 * \see `GetTileToRender()`
 *
 * \since 0.2.0
 */
[[nodiscard]] auto GetSourceRect(const entt::registry& registry,
                                 entt::entity tilesetEntity,
                                 TileID id) -> const cen::irect&;

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
 * \return the corresponding local tile identifier; `nothing` if something went
 * wrong.
 *
 * \since 0.2.0
 */
[[nodiscard]] auto ConvertToLocal(const entt::registry& registry, TileID global)
    -> Maybe<TileID>;

/// \} End of tileset system

}  // namespace Tactile::Sys
