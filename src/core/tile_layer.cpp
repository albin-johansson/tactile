#include "tile_layer.h"

#include <algorithm>

namespace tactile {

TileLayer::TileLayer(int nRows, int nCols)
{
  for (auto r = 0; r < nRows; ++r) {
    TileRow row;
    row.reserve(nCols);
    for (auto c = 0; c < nCols; ++c) {
      row.push_back(empty);
    }
    tiles.push_back(row);
  }
}

void TileLayer::add_row(TileID id, int nCols) noexcept
{
  std::vector<TileID> row;
  row.reserve(nCols);
  std::fill(row.begin(), row.end(), id);
  tiles.push_back(row);
}

void TileLayer::add_col(TileID id) noexcept
{
  for (auto& row : tiles) {
    row.push_back(id);
  }
}

}  // namespace tactile
