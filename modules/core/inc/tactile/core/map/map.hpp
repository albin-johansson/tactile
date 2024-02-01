// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/meta/meta_context.hpp"
#include "tactile/foundation/functional/maybe.hpp"
#include "tactile/foundation/math/vector.hpp"
#include "tactile/foundation/misc/matrix_extent.hpp"
#include "tactile/foundation/misc/uuid.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile::core {

struct TileFormat;
struct MapIdentifiers;
class GroupLayer;

/**
 * Basic interface implemented by all tilemap variations.
 */
class IMap : public IMetaContext {
 public:
  TACTILE_INTERFACE_CLASS(IMap);

  /**
   * Updates the size of the map (and all associated tile layers).
   *
   * \param extent The new map extent.
   */
  virtual void set_extent(const MatrixExtent& extent) = 0;

  /**
   * Returns the size of the map.
   *
   * \return
   *    A map extent.
   */
  [[nodiscard]]
  virtual auto extent() const -> const MatrixExtent& = 0;

  /**
   * Sets the logical size of tiles in the map.
   *
   * \param size The new logical tile size.
   */
  virtual void set_tile_size(Int2 size) = 0;

  /**
   * Returns the logical size of all tiles.
   *
   * \return
   *    A tile size.
   */
  [[nodiscard]]
  virtual auto tile_size() const -> Int2 = 0;

  /**
   * Marks a layer in the map as active.
   *
   * \note
   *    If the provided UUID isn't associated with a layer, then
   *    the active layer property is reset.
   *
   * \param layer_uuid The UUID of the layer to select.
   */
  virtual void set_active_layer(Maybe<UUID> layer_uuid) = 0;

  /**
   * Returns the UUID of the active layer.
   *
   * \return
   *    The UUID of the currently active layer, if any.
   */
  [[nodiscard]]
  virtual auto active_layer_uuid() const -> Maybe<UUID> = 0;

  /**
   * Returns the invisible root layer.
   *
   * \return
   *    The root group layer.
   */
  [[nodiscard]]
  virtual auto root_layer() -> GroupLayer& = 0;

  /** \copydoc root_layer() */
  [[nodiscard]]
  virtual auto root_layer() const -> const GroupLayer& = 0;

  /**
   * Returns the associated tile format configuration.
   *
   * \return
   *    A tile format configuration.
   */
  [[nodiscard]]
  virtual auto tile_format() -> TileFormat& = 0;

  /** \copydoc tile_format() */
  [[nodiscard]]
  virtual auto tile_format() const -> const TileFormat& = 0;

  /**
   * Returns the associated identifier state.
   *
   * \return
   *    The map's identifier state.
   */
  [[nodiscard]]
  virtual auto identifiers() -> MapIdentifiers& = 0;

  /** \copydoc identifiers() */
  [[nodiscard]]
  virtual auto identifiers() const -> const MapIdentifiers& = 0;
};

}  // namespace tactile::core