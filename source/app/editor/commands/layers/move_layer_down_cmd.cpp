#include "move_layer_down_cmd.hpp"

#include "core/systems/layers/layer_system.hpp"
#include "core/systems/layers/layer_tree_system.hpp"

namespace tactile {

MoveLayerDownCmd::MoveLayerDownCmd(registry_ref registry, const layer_id id)
    : command_base{"Move Layer Down"}
    , mRegistry{registry}
    , mLayerId{id}
{}

void MoveLayerDownCmd::undo()
{
  auto& registry = mRegistry.get();
  auto&& [entity, layer] = sys::get_layer(registry, mLayerId);
  sys::move_layer_up(registry, entity);
}

void MoveLayerDownCmd::redo()
{
  auto& registry = mRegistry.get();
  auto&& [entity, layer] = sys::get_layer(registry, mLayerId);
  sys::move_layer_down(registry, entity);
}

}  // namespace tactile
