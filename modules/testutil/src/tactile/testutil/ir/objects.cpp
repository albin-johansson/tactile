// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/testutil/ir/objects.hpp"

#include <fmt/format.h>

namespace tactile::testutil {

auto make_ir_object(const ObjectType type, const int id) -> ir::Object
{
  return {
    .meta = ir::Metadata {},
    .name = fmt::format("Object {}", id),
    .id = ObjectID {id},
    .type = type,
    .x = 0.0f,
    .y = 0.0f,
    .width = 0.0f,
    .height = 0.0f,
    .tag = "test_tag",
    .visible = true,
  };
}

}  // namespace tactile::testutil
