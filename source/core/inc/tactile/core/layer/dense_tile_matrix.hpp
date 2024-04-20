// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/vector.hpp"
#include "tactile/base/id.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/util/matrix_extent.hpp"
#include "tactile/core/util/matrix_index.hpp"

namespace tactile {

/**
 * Represents a densely populated two-dimensional grid of tile identifiers.
 */
class DenseTileMatrix final
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

  /**
   * Changes the extent of the matrix.
   *
   * \param new_extent The new extent.
   */
  void resize(const MatrixExtent& new_extent);

  /**
   * Returns the tile identifier at a given index.
   *
   * \warning
   * This function performs no bounds checking.
   *
   * \pre The provided index must be valid.
   *
   * \param index The index of the desired tile.
   *
   * \return
   * A tile identifier.
   */
  auto operator[](MatrixIndex index) noexcept -> TileID&;

  /**
   * Returns the tile identifier at a given index.
   *
   * \param index The index of the desired tile.
   *
   * \return
   * A tile identifier.
   *
   * \throw Exception if the index is invalid.
   */
  [[nodiscard]]
  auto at(MatrixIndex index) -> TileID;

  /**
   * \copydoc at
   */
  [[nodiscard]]
  auto at(MatrixIndex index) const -> const TileID&;

  /**
   * Indicates whether an index is valid, i.e., whether it refers to a tile in
   * the matrix.
   *
   * \param index The index that will be checked.
   *
   * \return
   * True if the index is valid; false otherwise.
   */
  [[nodiscard]]
  auto is_valid(const MatrixIndex& index) const noexcept -> bool;

  /**
   * Returns the current extent of the matrix.
   *
   * \return
   * A matrix extent.
   */
  [[nodiscard]]
  auto get_extent() const noexcept -> const MatrixExtent&;

  [[nodiscard]]
  auto operator==(const DenseTileMatrix&) const -> bool = default;

 private:
  MatrixExtent mExtent {0, 0};
  Vector<Vector<TileID>> mRows {};

  void _set_row_count(usize rows);

  void _set_column_count(usize cols);
};

}  // namespace tactile
