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
  sys::RemoveLayer(mRegistry, mNewLayerId.value());
}

void DuplicateLayerCmd::Redo()
{
  auto& registry = mRegistry.get();
  const auto entity = sys::DuplicateLayer(registry, mLayerId);

  if (!mNewLayerId) {
    mNewLayerId = sys::GetLayerId(registry, entity);
  }
  else {
    // Reuse previous ID of duplicated layer
    TACTILE_ASSERT(sys::FindLayer(registry, *mNewLayerId) == entt::null);
    auto& layer = registry.get<Layer>(entity);
    layer.id = *mNewLayerId;
  }
}

}  // namespace tactile