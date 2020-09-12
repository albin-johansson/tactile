#pragma once

#include <cassert>
#include <concepts>  // invocable

#include "map.hpp"
#include "position.hpp"
#include "tileset.hpp"

namespace tactile::core {

// map, tileset, tilePos, tilesetPos
template <typename T>
concept TilesetIterationCallback =
    std::invocable<T, const tileset&, const position&, const position&>;

template <TilesetIterationCallback T>
void iterate_tileset_selection(T&& callback,
                               const tileset& tileset,
                               const position& origin)
{
  assert(tileset.get_selection().has_value());

  const auto& [topLeft, bottomRight] = *tileset.get_selection();

  if (topLeft == bottomRight) {
    callback(tileset, origin, topLeft);
  } else {
    const auto nRows = 1_row + (bottomRight.get_row() - topLeft.get_row());
    const auto nCols = 1_col + (bottomRight.get_col() - topLeft.get_col());

    const auto mouseRow = origin.get_row();
    const auto mouseCol = origin.get_col();

    for (row r{0}; r < nRows; ++r) {
      for (col c{0}; c < nCols; ++c) {
        const position tilePos{mouseRow + r, mouseCol + c};
        const position tilesetPos = topLeft.offset(r, c);
        callback(tileset, tilePos, tilesetPos);
      }
    }
  }
}

}  // namespace tactile::core
