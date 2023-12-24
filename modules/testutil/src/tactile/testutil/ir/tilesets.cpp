// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/testutil/ir/tilesets.hpp"

#include <utility>  // move

namespace tactile::testutil {

auto make_dummy_ir_tileset(String name) -> ir::Tileset
{
  return ir::Tileset {
    .meta = ir::Metadata {},
    .name = std::move(name),
    .tile_width = 16,
    .tile_height = 8,
    .tile_count = 48,
    .column_count = 6,
    .image_width = 96,
    .image_height = 64,
    .image_path = "../../../images/dummy.png",
    .tiles = {},
    .is_embedded = false,
  };
}

auto make_ir_tile(const TileIndex index) -> ir::Tile
{
  return {
    .meta = ir::Metadata {},
    .index = index,
    .objects = Vector<ir::Object> {},
    .animation = ir::TileAnimation {},
  };
}

}  // namespace tactile::testutil
