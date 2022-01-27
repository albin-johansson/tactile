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
  sys::restore_layer(mRegistry, mLayerSnapshot.value());
}

void RemoveLayerCmd::Redo()
{
  const auto entity = sys::find_layer(mRegistry, mLayerId);
  TACTILE_ASSERT(entity != entt::null);

  mLayerSnapshot = sys::copy_layer(mRegistry, entity);
  sys::remove_layer(mRegistry, entity);
}

}  // namespace tactile
