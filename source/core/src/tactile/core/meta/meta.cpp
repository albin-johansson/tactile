// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/meta/meta.hpp"

#include "tactile/base/io/save/ir.hpp"
#include "tactile/core/debug/assert.hpp"
#include "tactile/core/entity/registry.hpp"

namespace tactile {

auto is_meta(const Registry& registry, const EntityID id) -> bool
{
  return registry.has<CMeta>(id);
}

void convert_ir_metadata(Registry& registry,
                         const EntityID meta_id,
                         const ir::Metadata& ir_metadata)
{
  TACTILE_ASSERT(is_meta(registry, meta_id));

  auto& meta = registry.get<CMeta>(meta_id);
  meta.name = ir_metadata.name;

  for (const auto& [prop_name, prop_value] : ir_metadata.properties) {
    meta.properties.insert_or_assign(prop_name, prop_value);
  }

  for (const auto& [comp_name, comp_attributes] : ir_metadata.components) {
    // TODO
  }
}

}  // namespace tactile
