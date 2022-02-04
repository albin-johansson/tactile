#include "move_layer_up_cmd.hpp"

#include "core/systems/layers/layer_system.hpp"
#include "core/systems/layers/layer_tree_system.hpp"

namespace tactile {

MoveLayerUpCmd::MoveLayerUpCmd(registry_ref registry, const layer_id id)
    : command_base{"Move Layer Up"}
    , mRegistry{registry}
    , mLayerId{id}
{}

void MoveLayerUpCmd::undo()
{
  auto& registry = mRegistry.get();

  auto&& [entity, layer] = sys::get_layer(registry, mLayerId);
  sys::move_layer_down(registry, entity);
}

void MoveLayerUpCmd::redo()
{
  auto& registry = mRegistry.get();

  auto&& [entity, layer] = sys::get_layer(registry, mLayerId);
  sys::move_layer_up(registry, entity);
}

}  // namespace tactile
