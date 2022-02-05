#include "move_layer_up_cmd.hpp"

#include "core/systems/layers/layer_system.hpp"
#include "core/systems/layers/layer_tree_system.hpp"

namespace tactile {

move_layer_up_cmd::move_layer_up_cmd(registry_ref registry, const layer_id id)
    : command_base{"Move Layer Up"}
    , mRegistry{registry}
    , mLayerId{id}
{}

void move_layer_up_cmd::undo()
{
  auto& registry = mRegistry.get();

  auto&& [entity, layer] = sys::get_layer(registry, mLayerId);
  sys::move_layer_down(registry, entity);
}

void move_layer_up_cmd::redo()
{
  auto& registry = mRegistry.get();

  auto&& [entity, layer] = sys::get_layer(registry, mLayerId);
  sys::move_layer_up(registry, entity);
}

}  // namespace tactile
