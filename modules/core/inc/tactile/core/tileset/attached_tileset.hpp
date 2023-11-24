// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/container/smart_ptr.hpp"
#include "tactile/core/functional/maybe.hpp"
#include "tactile/core/misc/id_types.hpp"
#include "tactile/core/prelude.hpp"
#include "tactile/core/tileset/tileset.hpp"

namespace tactile {

/**
 * \brief Represents a tileset that is attached to a map.
 *
 * \see `Tileset`
 */
class TACTILE_CORE_API AttachedTileset final {
 public:
  /**
   * \brief Creates an attached tileset.
   *
   * \param tileset       the associated tileset.
   * \param first_tile_id the global tile identifier to associate with the first tile.
   */
  AttachedTileset(Shared<Tileset> tileset, TileID first_tile_id);

  /**
   * \brief Converts a tile identifier to a tile index.
   *
   * \pre The tile identifier must be associated with the attached tileset.
   *
   * \param tile_id the tile identifier.
   *
   * \return a tile index.
   *
   * \see `has_tile()`
   */
  [[nodiscard]]
  auto to_index(TileID tile_id) const -> TileIndex;

  /**
   * \brief Indicates whether a tile identifier is contained in the tileset.
   *
   * \param tile_id the tile identifier.
   *
   * \return true if the identifier is valid; false otherwise.
   */
  [[nodiscard]]
  auto has_tile(TileID tile_id) const -> bool;

  /**
   * \brief Returns the first (lowest) associated tile identifier.
   *
   * \return a tile identifier.
   */
  [[nodiscard]]
  auto get_first_tile_id() const -> TileID;

  /**
   * \brief Returns the last (greatest) associated tile identifier.
   *
   * \return a tile identifier.
   */
  [[nodiscard]]
  auto get_last_tile_id() const -> TileID;

  /**
   * \brief Returns the associated tileset.
   *
   * \return a tileset reference.
   */
  [[nodiscard]]
  auto get_tileset() -> Tileset&;

  /**
   * \copydoc get_tileset()
   */
  [[nodiscard]]
  auto get_tileset() const -> const Tileset&;

 private:
  Shared<Tileset> mTileset;
  TileID mFirstTileId;
  TileID mLastTileId;
};

}  // namespace tactile
