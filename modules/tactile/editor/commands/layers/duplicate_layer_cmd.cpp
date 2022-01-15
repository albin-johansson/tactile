#include "duplicate_layer_cmd.hpp"

#include "assert.hpp"
#include "core/systems/layers/layer_system.hpp"

namespace Tactile {

DuplicateLayerCmd::DuplicateLayerCmd(RegistryRef registry, const LayerID id)
    : ACommand{"Duplicate Layer"}
    , mRegistry{registry}
    , mLayerId{id}
{}

void DuplicateLayerCmd::Undo()
{
  Sys::RemoveLayer(mRegistry, mNewLayerId.value());
}

void DuplicateLayerCmd::Redo()
{
  auto& registry = mRegistry.get();
  const auto entity = Sys::DuplicateLayer(registry, mLayerId);

  if (!mNewLayerId) {
    mNewLayerId = Sys::GetLayerId(registry, entity);
  }
  else {
    // Reuse previous ID of duplicated layer
    TACTILE_ASSERT(Sys::FindLayer(registry, *mNewLayerId) == entt::null);
    auto& layer = registry.get<Layer>(entity);
    layer.id = *mNewLayerId;
  }
}

}  // namespace Tactile