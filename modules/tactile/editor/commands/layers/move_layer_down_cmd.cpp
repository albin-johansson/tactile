#include "move_layer_down_cmd.hpp"

#include "core/systems/layers/layer_system.hpp"

namespace tactile {

MoveLayerDownCmd::MoveLayerDownCmd(RegistryRef registry, const LayerID id)
    : ACommand{"Move Layer Down"}
    , mRegistry{registry}
    , mLayerId{id}
{}

void MoveLayerDownCmd::Undo()
{
  auto& registry = mRegistry.get();
  auto&& [entity, layer] = sys::get_layer(registry, mLayerId);
  sys::move_layer_up(registry, entity);
}

void MoveLayerDownCmd::Redo()
{
  auto& registry = mRegistry.get();
  auto&& [entity, layer] = sys::get_layer(registry, mLayerId);
  sys::move_layer_down(registry, entity);
}

}  // namespace tactile
