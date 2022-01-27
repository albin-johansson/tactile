#include "duplicate_layer_cmd.hpp"

#include "assert.hpp"
#include "core/systems/layers/layer_system.hpp"

namespace tactile {

DuplicateLayerCmd::DuplicateLayerCmd(RegistryRef registry, const LayerID id)
    : ACommand{"Duplicate Layer"}
    , mRegistry{registry}
    , mLayerId{id}
{}

void DuplicateLayerCmd::Undo()
{
  auto& registry = mRegistry.get();

  auto&& [entity, layer] = sys::get_layer(registry, mLayerId);
  sys::remove_layer(registry, entity);
}

void DuplicateLayerCmd::Redo()
{
  auto& registry = mRegistry.get();

  const auto sourceEntity = sys::find_layer(registry, mLayerId);
  const auto entity = sys::duplicate_layer(registry, sourceEntity);

  if (!mNewLayerId) {
    const auto& layer = registry.get<Layer>(entity);
    mNewLayerId = layer.id;
  }
  else {
    // Reuse previous ID of duplicated layer
    TACTILE_ASSERT(sys::find_layer(registry, *mNewLayerId) == entt::null);
    auto& layer = registry.get<Layer>(entity);
    layer.id = *mNewLayerId;
  }
}

}  // namespace tactile