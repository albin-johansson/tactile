// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/container/vector.hpp"
#include "tactile/foundation/io/ir.hpp"
#include "tactile/foundation/misc/tile_matrix.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile::testutil {

[[nodiscard]]
auto make_ir_tile_layer(int id, MatrixExtent extent) -> ir::Layer;

[[nodiscard]]
auto make_ir_object_layer(int id) -> ir::Layer;

[[nodiscard]]
auto make_ir_group_layer(int id, Vector<ir::Layer> layers = {}) -> ir::Layer;

[[nodiscard]]
auto make_tile_matrix_with_increasing_tiles(MatrixExtent extent, int32 first_id = 1) -> TileMatrix;

[[nodiscard]]
auto make_complex_ir_group_layer(int id, MatrixExtent extent) -> ir::Layer;

}  // namespace tactile::testutil
