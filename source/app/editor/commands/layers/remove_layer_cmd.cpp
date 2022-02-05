#include "remove_layer_cmd.hpp"

#include "core/systems/layers/layer_system.hpp"
#include "misc/assert.hpp"

namespace tactile {

remove_layer_cmd::remove_layer_cmd(registry_ref registry, const layer_id id)
    : command_base{"Remove Layer"}
    , mRegistry{registry}
    , mLayerId{id}
{}

void remove_layer_cmd::undo()
{
  sys::restore_layer(mRegistry, mLayerSnapshot.value());
}

void remove_layer_cmd::redo()
{
  const auto entity = sys::find_layer(mRegistry, mLayerId);
  TACTILE_ASSERT(entity != entt::null);

  mLayerSnapshot = sys::remove_layer(mRegistry, entity);
}

}  // namespace tactile
