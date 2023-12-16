// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/io/ir.hpp"
#include "tactile/foundation/misc/tile_matrix.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile::testutil {

[[nodiscard]]
auto make_ir_map(String name, MatrixExtent extent) -> ir::Map;

}  // namespace tactile::testutil
