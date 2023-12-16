// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/testutil/ir/tilesets.hpp"

#include <utility>  // move

namespace tactile::testutil {

auto make_dummy_ir_tileset(String name) -> ir::Tileset
{
  ir::Tileset tileset {};

  tileset.name = std::move(name);
  tileset.tile_width = 16;
  tileset.tile_height = 8;
  tileset.tile_count = 48;
  tileset.column_count = 6;
  tileset.image_width = 96;
  tileset.image_height = 64;
  tileset.image_path = "../../../images/dummy.png";

  return tileset;
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
