#include "duplicate_layer_cmd.hpp"

#include "assert.hpp"
#include "core/systems/layers/layer_system.hpp"

namespace tactile {

DuplicateLayerCmd::DuplicateLayerCmd(registry_ref registry, const layer_id id)
    : command_base{"Duplicate Layer"}
    , mRegistry{registry}
    , mLayerId{id}
{}

void DuplicateLayerCmd::undo()
{
  auto& registry = mRegistry.get();

  auto&& [entity, layer] = sys::get_layer(registry, mLayerId);
  sys::remove_layer(registry, entity);
}

void DuplicateLayerCmd::redo()
{
  auto& registry = mRegistry.get();

  const auto sourceEntity = sys::find_layer(registry, mLayerId);
  const auto entity = sys::duplicate_layer(registry, sourceEntity);

  if (!mNewLayerId) {
    const auto& layer = registry.get<comp::layer>(entity);
    mNewLayerId = layer.id;
  }
  else {
    // Reuse previous ID of duplicated layer
    TACTILE_ASSERT(sys::find_layer(registry, *mNewLayerId) == entt::null);
    auto& layer = registry.get<comp::layer>(entity);
    layer.id = *mNewLayerId;
  }
}

}  // namespace tactile