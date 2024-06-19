// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/hash_map.hpp"
#include "tactile/base/id.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/layer/tile_matrix.hpp"
#include "tactile/core/util/matrix_extent.hpp"
#include "tactile/core/util/matrix_index.hpp"

namespace tactile {

/**
 * Represents a sparsely populated two-dimensional grid of tile identifiers.
 */
class SparseTileMatrix final : public ITileMatrix
{
 public:
  TACTILE_DEFAULT_COPY(SparseTileMatrix);
  TACTILE_DEFAULT_MOVE(SparseTileMatrix);

  /**
   * Creates an empty tile matrix with extent (0, 0).
   */
  SparseTileMatrix() noexcept = default;

  /**
   * Creates an empty matrix with a given extent.
   *
   * \param extent The initial extent.
   */
  explicit SparseTileMatrix(const MatrixExtent& extent);

  ~SparseTileMatrix() noexcept = default;

  void resize(const MatrixExtent& new_extent) override;

  [[nodiscard]]
  auto operator[](const MatrixIndex& index) -> TileID& override;

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
  HashMap<MatrixIndex, TileID> mTiles {};
};

}  // namespace tactile
