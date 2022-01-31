#include "remove_layer_cmd.hpp"

#include "assert.hpp"
#include "core/systems/layers/layer_system.hpp"

namespace tactile {

RemoveLayerCmd::RemoveLayerCmd(registry_ref registry, const layer_id id)
    : command_base{"Remove Layer"}
    , mRegistry{registry}
    , mLayerId{id}
{}

void RemoveLayerCmd::undo()
{
  sys::restore_layer(mRegistry, mLayerSnapshot.value());
}

void RemoveLayerCmd::redo()
{
  const auto entity = sys::find_layer(mRegistry, mLayerId);
  TACTILE_ASSERT(entity != entt::null);

  mLayerSnapshot = sys::remove_layer(mRegistry, entity);
}

}  // namespace tactile
