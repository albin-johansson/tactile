// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/vector.hpp"
#include "tactile/base/id.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/layer/tile_matrix.hpp"
#include "tactile/core/util/matrix_extent.hpp"
#include "tactile/core/util/matrix_index.hpp"

namespace tactile {

/**
 * Represents a densely populated two-dimensional grid of tile identifiers.
 *
 * \ingroup Layer
 */
class DenseTileMatrix final : public ITileMatrix
{
 public:
  TACTILE_DEFAULT_COPY(DenseTileMatrix);
  TACTILE_DEFAULT_MOVE(DenseTileMatrix);

  /**
   * Creates an empty tile matrix with extent (0, 0).
   */
  DenseTileMatrix() noexcept = default;

  /**
   * Creates an empty tile matrix with a given extent.
   *
   * \param extent The initial extent.
   */
  explicit DenseTileMatrix(const MatrixExtent& extent);

  ~DenseTileMatrix() noexcept = default;

  void resize(const MatrixExtent& new_extent) override;

  auto operator[](const MatrixIndex& index) noexcept -> TileID& override;

  [[nodiscard]]
  auto at(const MatrixIndex& index) -> TileID override;

  [[nodiscard]]
  auto at(const MatrixIndex& index) const -> TileID override;

  [[nodiscard]]
  auto is_valid(const MatrixIndex& index) const noexcept -> bool override;

  [[nodiscard]]
  auto get_extent() const noexcept -> const MatrixExtent& override;

 private:
  MatrixExtent mExtent {0, 0};
  Vector<Vector<TileID>> mRows {};

  void _set_row_count(MatrixExtent::value_type rows);

  void _set_column_count(MatrixExtent::value_type cols);
};

}  // namespace tactile
