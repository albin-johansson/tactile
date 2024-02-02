// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tileset_helpers.hpp"

namespace tactile::core::test {

auto make_dummy_tileset_info() -> TilesetCreateInfo
{
  const int32 tile_count = 1'000;

  const int32 col_count = 25;
  const int32 row_count = tile_count / col_count;

  const Int2 tile_size {12, 34};
  const Int2 texture_size {col_count * tile_size.x(), row_count * tile_size.y()};

  return {
    .tile_count = tile_count,
    .column_count = col_count,
    .tile_size = tile_size,
    .texture_size = texture_size,
    .texture_uuid = UUID::generate(),
  };
}

}  // namespace tactile::core::test
