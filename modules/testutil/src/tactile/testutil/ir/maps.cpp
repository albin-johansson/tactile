// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/testutil/ir/maps.hpp"

#include <utility>  // move

namespace tactile::testutil {

auto make_ir_map(String name, const MatrixExtent extent) -> ir::Map
{
  ir::Map map {};
  map.name = std::move(name);

  map.row_count = extent.row_count;
  map.col_count = extent.col_count;

  map.tile_width = 32;
  map.tile_height = 48;

  map.next_layer_id = 100;
  map.next_object_id = 200;

  map.tile_format.encoding = TileEncoding::kPlainText;
  map.tile_format.compression = CompressionMode::kNone;

  return map;
}

}  // namespace tactile::testutil
