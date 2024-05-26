// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/span.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/numeric/vec.hpp"
#include "tactile/core/util/matrix_index.hpp"

namespace tactile::ui {

/**
 * Provides pre-computed information about hexagons of a specific size.
 *
 * \ingroup UI
 */
struct HexagonInfo final
{
  /** The length of the six side segments. */
  float side_length;

  /** The distance from the center point to the midpoint of a side. */
  float apothem;

  /** The length of diagonals that don't cross the center point. */
  float short_diagonal;

  /** The length of diagonals that cross the center point. */
  float long_diagonal;

  [[nodiscard]]
  constexpr auto width() const noexcept -> float
  {
    return short_diagonal;
  }

  [[nodiscard]]
  constexpr auto height() const noexcept -> float
  {
    return long_diagonal;
  }
};

/**
 * Computes basic hexagon information for a given side length.
 *
 * \param side_length The length of each side segment.
 *
 * \return
 * The computed hexagon information.
 *
 * \ingroup UI
 */
[[nodiscard]]
auto get_hexagon_info(float side_length) noexcept -> HexagonInfo;

/**
 * Computes the vertices of a hexagon at a given position.
 *
 * \details
 * The vertices are written in clockwise order, starting at the top vertex.
 *
 * \note
 * This function assumes that the hexagon is rotated by half Pi.
 *
 * \pre The provided span must have at least six elements.
 *
 * \param      center_pos       The center position of the hexagon.
 * \param      hexagon_info     The hexagon information.
 * \param[out] hexagon_vertices The computed vertices.
 *
 * \ingroup UI
 */
void get_hexagon_vertices(const Float2& center_pos,
                          const HexagonInfo& hexagon_info,
                          Span<Float2> hexagon_vertices);

/**
 * Computes the bounding box for a hexagon at a given position.
 *
 * \note
 * This function assumes that the hexagon is rotated by half Pi.
 *
 * \param center_pos   The center position of the hexagon.
 * \param hexagon_info The hexagon information.
 *
 * \return
 * A bounding box for the hexagon.
 *
 * \ingroup UI
 */
[[nodiscard]]
auto get_hexagon_bounding_box(const Float2& center_pos,
                              const HexagonInfo& hexagon_info) noexcept
    -> Float4;

/**
 * Returns the center of a hexagon located at a given matrix index.
 *
 * \param index        The matrix index to query.
 * \param hexagon_info The hexagon information.
 *
 * \return
 * The hexagon center point.
 *
 * \ingroup UI
 */
[[nodiscard]]
auto get_hexagon_center(const MatrixIndex& index,
                        const HexagonInfo& hexagon_info) noexcept -> Float2;

}  // namespace tactile::ui
