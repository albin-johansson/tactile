// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <compare>  // strong_ordering

#include "tactile/foundation/api.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * \brief Represents a position in a tile grid as a row/column pair.
 *
 * \note Negative indices are allowed to be able to represent offsets.
 */
struct TACTILE_FOUNDATION_API TilePos final {
  ssize row {};  ///< The row index.
  ssize col {};  ///< The column index.

  [[nodiscard]] constexpr auto operator<=>(const TilePos&) const noexcept
      -> std::strong_ordering = default;
};

/**
 * \brief Adds two tile positions by performing a component-wise addition.
 *
 * \param p1 the left-hand side position.
 * \param p2 the right-hand side position.
 *
 * \return a new position.
 */
[[nodiscard]] constexpr auto operator+(const TilePos& p1, const TilePos& p2) noexcept
    -> TilePos
{
  return {p1.row + p2.row, p1.col + p2.col};
}

/**
 * \brief Subtracts two tile positions by performing a component-wise subtraction.
 *
 * \param p1 the left-hand side position.
 * \param p2 the right-hand side position.
 *
 * \return a new position.
 */
[[nodiscard]] constexpr auto operator-(const TilePos& p1, const TilePos& p2) noexcept
    -> TilePos
{
  return {p1.row - p2.row, p1.col - p2.col};
}

/**
 * \brief Updates a tile position by adding another tile position to it.
 *
 * \param p1 the left-hand side position.
 * \param p2 the right-hand side position.
 *
 * \return the updated position.
 */
constexpr auto operator+=(TilePos& p1, const TilePos& p2) noexcept -> TilePos&
{
  p1 = p1 + p2;
  return p1;
}

/**
 * \brief Updates a tile position by subtracting another tile position from it.
 *
 * \param p1 the left-hand side position.
 * \param p2 the right-hand side position.
 *
 * \return the updated position.
 */
constexpr auto operator-=(TilePos& p1, const TilePos& p2) noexcept -> TilePos&
{
  p1 = p1 - p2;
  return p1;
}

}  // namespace tactile