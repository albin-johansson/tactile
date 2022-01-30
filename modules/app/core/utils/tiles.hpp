#pragma once

#include "core/components/layer.hpp"
#include "tactile_def.hpp"

namespace tactile {

[[nodiscard]] auto make_tile_row(usize nCols) -> tile_row;

/**
 * \brief Creates a tile matrix with the specified dimensions.
 *
 * \param nRows the number or rows.
 * \param nCols the number of columns.
 *
 * \return a matrix with empty tile identifiers.
 */
[[nodiscard]] auto make_tile_matrix(usize nRows, usize nCols) -> tile_matrix;

}  // namespace tactile
