// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <compare>  // strong_ordering

#include "tactile/foundation/functional/hash.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * Represents a position in a tile grid as a row/column pair.
 *
 * \note
 *    Negative indices are allowed to be able to represent offsets.
 */
struct TilePos final {
  ssize row {};  ///< The row index.
  ssize col {};  ///< The column index.

  [[nodiscard]] constexpr auto operator<=>(const TilePos&) const noexcept
      -> std::strong_ordering = default;
};

/**
 * Adds two tile positions by performing a component-wise addition.
 *
 * \param p1 The left-hand side position.
 * \param p2 The right-hand side position.
 *
 * \return
 *    A new position.
 */
[[nodiscard]] constexpr auto operator+(const TilePos& p1, const TilePos& p2) noexcept
    -> TilePos
{
  return {p1.row + p2.row, p1.col + p2.col};
}

/**
 * Subtracts two tile positions by performing a component-wise subtraction.
 *
 * \param p1 The left-hand side position.
 * \param p2 The right-hand side position.
 *
 * \return
 *    A new position.
 */
[[nodiscard]] constexpr auto operator-(const TilePos& p1, const TilePos& p2) noexcept
    -> TilePos
{
  return {p1.row - p2.row, p1.col - p2.col};
}

/**
 * Updates a tile position by adding another tile position to it.
 *
 * \param p1 The left-hand side position.
 * \param p2 The right-hand side position.
 *
 * \return
 *    The updated position.
 */
constexpr auto operator+=(TilePos& p1, const TilePos& p2) noexcept -> TilePos&
{
  p1 = p1 + p2;
  return p1;
}

/**
 * Updates a tile position by subtracting another tile position from it.
 *
 * \param p1 The left-hand side position.
 * \param p2 The right-hand side position.
 *
 * \return
 *    The updated position.
 */
constexpr auto operator-=(TilePos& p1, const TilePos& p2) noexcept -> TilePos&
{
  p1 = p1 - p2;
  return p1;
}

}  // namespace tactile

TACTILE_IMPLEMENT_HASH(tactile::TilePos, value.row, value.col);
