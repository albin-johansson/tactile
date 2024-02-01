// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/layer/tile_layer.hpp"
#include "tactile/foundation/misc/tile_matrix.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile::core {

/**
 * A tile layer variant based on conventional two-dimensional arrays.
 *
 * \details
 *    Dense tile layers use two-dimensional arrays as their representation
 *    of the modelled tile matrix. This representation is highly efficient
 *    for both iteration and read/write access. However, dense tile layers
 *    can waste quite a lot of memory when they are nearly empty, since
 *    the memory consumption is the same regardless of the content.
 *    Therefore, dense tile layers should ideally only be used for
 *    densely populated tile layers, such as ground layers.
 *
 * \see SparseTileLayer
 */
class TACTILE_CORE_API DenseTileLayer final : public TileLayer {
 public:
  /**
   * Creates an empty dense tile layer.
   *
   * \param row_count The number of tile rows, must be greater than zero.
   * \param col_count The number of tile columns, must be greater than zero.
   */
  DenseTileLayer(ssize row_count, ssize col_count);

  /**
   * Creates an empty dense tile layer.
   *
   * \param extent The initial extent, must be at least 1x1.
   */
  explicit DenseTileLayer(MatrixExtent extent);

  auto set_tile(const TilePos& pos, TileID id) -> Result<void> override;

  [[nodiscard]]
  auto tile_at(const TilePos& pos) const -> Maybe<TileID> override;

  auto set_extent(const MatrixExtent& extent) -> Result<void> override;

  [[nodiscard]]
  auto extent() const -> MatrixExtent override;

  [[nodiscard]]
  auto clone() const -> Shared<ILayer> override;

 private:
  MatrixExtent mExtent;
  TileMatrix mTileMatrix;

  void _add_row();
  void _add_column();
  void _remove_row();
  void _remove_column();
};

}  // namespace tactile::core
