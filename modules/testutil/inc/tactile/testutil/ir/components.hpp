// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/container/vector.hpp"
#include "tactile/foundation/io/ir.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile::testutil {

[[nodiscard]]
auto make_empty_ir_component(String name) -> ir::Component;

[[nodiscard]]
auto make_ir_component_with_basic_properties(String name) -> ir::Component;

[[nodiscard]]
auto make_ir_component_with_all_properties(String name) -> ir::Component;

[[nodiscard]]
auto make_ir_attached_component(const ir::Component& prototype) -> ir::AttachedComponent;

}  // namespace tactile::testutil
