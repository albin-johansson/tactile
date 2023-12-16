// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/io/ir.hpp"
#include "tactile/foundation/misc/object_type.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile::testutil {

[[nodiscard]]
auto make_ir_object(ObjectType type, int id) -> ir::Object;

}  // namespace tactile::testutil
