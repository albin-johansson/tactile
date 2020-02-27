#pragma once

#include <vector>

#include "tile.h"

namespace tactile {

class TileLayer final {
 public:
  TileLayer(int nRows, int nCols);

  void flood();

  void add_row(TileID id, int nCols) noexcept;

  void add_col(TileID id) noexcept;

 private:
  using TileRow = std::vector<int>;
  using TileMatrix = std::vector<std::vector<int>>;

  TileMatrix tiles;
};

}  // namespace tactile
