#include "move_layer_up_cmd.hpp"

#include "core/systems/layers/layer_system.hpp"

namespace tactile {

MoveLayerUpCmd::MoveLayerUpCmd(RegistryRef registry, const LayerID id)
    : ACommand{"Move Layer Up"}
    , mRegistry{registry}
    , mLayerId{id}
{}

void MoveLayerUpCmd::Undo()
{
  sys::MoveLayerDown(mRegistry, mLayerId);
}

void MoveLayerUpCmd::Redo()
{
  sys::MoveLayerUp(mRegistry, mLayerId);
}

}  // namespace tactile
