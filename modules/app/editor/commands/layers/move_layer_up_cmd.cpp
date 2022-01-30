#include "move_layer_up_cmd.hpp"

#include "core/systems/layers/layer_system.hpp"
#include "core/systems/layers/layer_tree_system.hpp"

namespace tactile {

MoveLayerUpCmd::MoveLayerUpCmd(RegistryRef registry, const layer_id id)
    : ACommand{"Move Layer Up"}
    , mRegistry{registry}
    , mLayerId{id}
{}

void MoveLayerUpCmd::Undo()
{
  auto& registry = mRegistry.get();

  auto&& [entity, layer] = sys::get_layer(registry, mLayerId);
  sys::move_layer_down(registry, entity);
}

void MoveLayerUpCmd::Redo()
{
  auto& registry = mRegistry.get();

  auto&& [entity, layer] = sys::get_layer(registry, mLayerId);
  sys::move_layer_up(registry, entity);
}

}  // namespace tactile
