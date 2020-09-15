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
    const auto diff = bottomRight - topLeft;
    const auto endRow = 1_row + diff.row();
    const auto endCol = 1_col + diff.col();
    for (row_t row{0}; row < endRow; ++row) {
      for (col_t col{0}; col < endCol; ++col) {
        const auto tilePos = origin.offset_by(row, col);
        const auto tilesetPos = topLeft.offset_by(row, col);
        callback(tileset, tilePos, tilesetPos);
      }
    }
  }
}

}  // namespace tactile::core
