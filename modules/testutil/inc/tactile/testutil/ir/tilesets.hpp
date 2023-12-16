// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/io/ir.hpp"
#include "tactile/foundation/misc/id_types.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile::testutil {

[[nodiscard]]
auto make_dummy_ir_tileset(String name) -> ir::Tileset;

[[nodiscard]]
auto make_ir_tile(TileIndex index) -> ir::Tile;

}  // namespace tactile::testutil
