// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/container/vector.hpp"
#include "tactile/foundation/io/ir.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile::testutil {

[[nodiscard]]
auto make_basic_properties() -> Vector<ir::NamedAttribute>;

[[nodiscard]]
auto make_all_properties() -> Vector<ir::NamedAttribute>;

}  // namespace tactile::testutil
