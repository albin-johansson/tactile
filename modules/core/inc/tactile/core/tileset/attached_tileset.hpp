// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/document/document_viewport.hpp"
#include "tactile/core/tileset/tileset.hpp"
#include "tactile/foundation/container/smart_ptr.hpp"
#include "tactile/foundation/functional/maybe.hpp"
#include "tactile/foundation/misc/id_types.hpp"
#include "tactile/foundation/prelude.hpp"

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
   * \param  tile_id the tile identifier.
   * \return a tile index.
   */
  [[nodiscard]]
  auto to_index(TileID tile_id) const -> Maybe<TileIndex>;

  /**
   * \brief Indicates whether a tile identifier is contained in the tileset.
   *
   * \param  tile_id the tile identifier.
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
  auto tileset() -> Tileset&;

  /** \copydoc tileset() */
  [[nodiscard]]
  auto tileset() const -> const Tileset&;

  /**
   * \brief Returns the viewport used when displaying the tileset in the tileset dock.
   *
   * \return the associated viewport (not the document viewport).
   */
  [[nodiscard]]
  auto viewport() -> DocumentViewport&;

  /** \copydoc viewport() */
  [[nodiscard]]
  auto viewport() const -> const DocumentViewport&;

 private:
  Shared<Tileset> mTileset;
  TileID mFirstTileId;
  TileID mLastTileId;
  DocumentViewport mViewport {};
};

}  // namespace tactile
