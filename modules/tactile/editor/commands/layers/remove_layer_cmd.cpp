#include "remove_layer_cmd.hpp"

#include "assert.hpp"
#include "core/systems/layers/layer_system.hpp"

namespace tactile {

RemoveLayerCmd::RemoveLayerCmd(RegistryRef registry, const LayerID id)
    : ACommand{"Remove Layer"}
    , mRegistry{registry}
    , mLayerId{id}
{}

void RemoveLayerCmd::Undo()
{
  sys::RestoreLayer(mRegistry, mLayerSnapshot.value());
}

void RemoveLayerCmd::Redo()
{
  const auto entity = sys::FindLayer(mRegistry, mLayerId);
  TACTILE_ASSERT(entity != entt::null);

  mLayerSnapshot = sys::CopyLayer(mRegistry, entity);
  sys::RemoveLayer(mRegistry, entity);
}

}  // namespace tactile
