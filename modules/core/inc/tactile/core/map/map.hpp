// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/context/meta_context.hpp"
#include "tactile/foundation/math/vector.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

struct MatrixExtent;
struct TileFormat;
class GroupLayer;

/**
 * \interface IMap
 * \brief Basic interface implemented by all tilemap variations.
 */
class IMap : public IMetaContext {
 public:
  TACTILE_INTERFACE_CLASS(IMap);

  /**
   * \brief Updates the size of the map (and all associated tile layers).
   *
   * \param extent the new map extent.
   */
  virtual void set_extent(const MatrixExtent& extent) = 0;

  /**
   * \brief Returns the size of the map.
   *
   * \return a map extent.
   */
  [[nodiscard]]
  virtual auto extent() const -> const MatrixExtent& = 0;

  /**
   * \brief Sets the logical size of tiles in the map.
   *
   * \param size the new logical tile size.
   */
  virtual void set_tile_size(Int2 size) = 0;

  /**
   * \brief Returns the logical size of all tiles.
   *
   * \return a tile size.
   */
  [[nodiscard]]
  virtual auto tile_size() const -> Int2 = 0;

  /**
   * \brief Returns the invisible root layer.
   *
   * \return the root group layer.
   */
  [[nodiscard]]
  virtual auto root_layer() -> GroupLayer& = 0;

  /** \copydoc root_layer() */
  [[nodiscard]]
  virtual auto root_layer() const -> const GroupLayer& = 0;

  /**
   * \brief Returns the associated tile format configuration.
   *
   * \return a tile format configuration.
   */
  [[nodiscard]]
  virtual auto tile_format() -> TileFormat& = 0;

  /** \copydoc tile_format() */
  [[nodiscard]]
  virtual auto tile_format() const -> const TileFormat& = 0;
};

}  // namespace tactile