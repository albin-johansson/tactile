#include "move_layer_down_cmd.hpp"

#include "core/systems/layers/layer_system.hpp"

namespace Tactile {

MoveLayerDownCmd::MoveLayerDownCmd(Ref<entt::registry> registry, const LayerID id)
    : ACommand{"Move Layer Down"}
    , mRegistry{registry}
    , mLayerId{id}
{}

void MoveLayerDownCmd::Undo()
{
  Sys::MoveLayerUp(mRegistry, mLayerId);
}

void MoveLayerDownCmd::Redo()
{
  Sys::MoveLayerDown(mRegistry, mLayerId);
}

}  // namespace Tactile
