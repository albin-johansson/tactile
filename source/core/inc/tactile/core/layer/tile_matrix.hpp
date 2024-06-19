// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/id.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile {

struct MatrixExtent;
struct MatrixIndex;

/**
 * Interface for tile matrix variants.
 */
class ITileMatrix
{
 public:
  TACTILE_INTERFACE_CLASS(ITileMatrix);

  /**
   * Changes the extent of the matrix.
   *
   * \param new_extent The new extent.
   */
  virtual void resize(const MatrixExtent& new_extent) = 0;

  /**
   * Returns the tile identifier at a given index.
   *
   * \pre The provided index must be valid.
   *
   * \param index The index of the desired tile.
   *
   * \return
   * A tile identifier.
   */
  virtual auto operator[](const MatrixIndex& index) -> TileID& = 0;

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
  virtual auto at(const MatrixIndex& index) -> TileID = 0;

  /**
   * \copydoc at
   */
  [[nodiscard]]
  virtual auto at(const MatrixIndex& index) const -> TileID = 0;

  /**
   * Indicates whether an index refers to a tile in the matrix.
   *
   * \param index The index that will be checked.
   *
   * \return
   * True if the index is valid; false otherwise.
   */
  [[nodiscard]]
  virtual auto is_valid(const MatrixIndex& index) const -> bool = 0;

  /**
   * Returns the current extent of the matrix.
   *
   * \return
   * A matrix extent.
   */
  [[nodiscard]]
  virtual auto get_extent() const -> const MatrixExtent& = 0;
};

}  // namespace tactile
