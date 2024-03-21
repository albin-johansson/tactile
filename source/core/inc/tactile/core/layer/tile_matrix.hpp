// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <ostream>  // ostream

#include "tactile/base/container/vector.hpp"
#include "tactile/base/id.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile {

/**
 * Represents a location in a two-dimensional matrix.
 */
struct MatrixIndex final {
  usize row;  ///< The row index.
  usize col;  ///< The column index.

  [[nodiscard]] auto operator==(const MatrixIndex&) const noexcept -> bool = default;
};

/**
 * Outputs a matrix index to a stream.
 *
 * \param stream The output stream.
 * \param index  The matrix index to emit.
 *
 * \return
 *    The provided stream.
 */
auto operator<<(std::ostream& stream, const MatrixIndex& index) -> std::ostream&;

/**
 * Represents the size of a two-dimensional matrix.
 */
struct MatrixExtent final {
  usize rows;  ///< The number of rows.
  usize cols;  ///< The number of columns.

  [[nodiscard]] auto operator==(const MatrixExtent&) const noexcept -> bool = default;
};

/**
 * Outputs a matrix extent to a stream.
 *
 * \param stream The output stream.
 * \param extent The matrix extent to emit.
 *
 * \return
 *    The provided stream.
 */
auto operator<<(std::ostream& stream, const MatrixExtent& extent) -> std::ostream&;

/**
 * Represents a two-dimensional grid of tile identifiers.
 */
class TileMatrix final {
 public:
  /**
   * Creates an empty tile matrix with extent (0, 0).
   */
  TileMatrix() noexcept = default;

  /**
   * Creates an empty tile matrix with a given extent.
   *
   * \param extent The initial extent.
   */
  explicit TileMatrix(const MatrixExtent& extent);

  TACTILE_DEFAULT_COPY(TileMatrix);
  TACTILE_DEFAULT_MOVE(TileMatrix);

  ~TileMatrix() noexcept = default;

  /**
   * Changes the extent of the matrix.
   *
   * \param new_extent The new extent.
   */
  void resize(const MatrixExtent& new_extent);

  /**
   * Sets the number of rows in the matrix.
   *
   * \param rows The new number of rows.
   */
  void set_row_count(usize rows);

  /**
   * Sets the number of columns in the matrix.
   *
   * \param cols The new number of columns.
   */
  void set_column_count(usize cols);

  /**
   * Returns the tile identifier at a given index.
   *
   * \param index The index of the desired tile.
   *
   * \return
   *    A tile identifier.
   *
   * \throw Exception if the index is invalid.
   */
  [[nodiscard]]
  auto at(MatrixIndex index) -> TileID&;

  /**
   * \copydoc at
   */
  [[nodiscard]]
  auto at(MatrixIndex index) const -> TileID;

  /**
   * Returns the tile identifier at a given index.
   *
   * \note
   *    This function performs no bounds checking.
   *
   * \pre
   *    The provided index must be valid.
   *
   * \param index The index of the desired tile.
   *
   * \return
   *    A tile identifier.
   */
  [[nodiscard]] auto operator[](MatrixIndex index) noexcept -> TileID&;

  /**
   * \copydoc operator[]
   */
  [[nodiscard]] auto operator[](MatrixIndex index) const noexcept -> TileID;

  /**
   * Indicates whether an index is valid, i.e., whether it refers to a tile in the matrix.
   *
   * \param index The index that will be checked.
   *
   * \return
   *    True if the index is valid; false otherwise.
   */
  [[nodiscard]]
  auto is_valid(const MatrixIndex& index) const noexcept -> bool;

  /**
   * Returns the current extent of the matrix.
   *
   * \return
   *    A matrix extent.
   */
  [[nodiscard]]
  auto get_extent() const noexcept -> const MatrixExtent&;

 private:
  MatrixExtent mExtent {0, 0};
  Vector<Vector<TileID>> mRows {};
};

}  // namespace tactile
