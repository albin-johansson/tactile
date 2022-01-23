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
  sys::MoveLayerUp(mRegistry, mLayerId);
}

void MoveLayerDownCmd::Redo()
{
  sys::MoveLayerDown(mRegistry, mLayerId);
}

}  // namespace tactile
