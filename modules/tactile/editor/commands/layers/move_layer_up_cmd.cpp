#include "move_layer_up_cmd.hpp"

#include "core/systems/layers/layer_system.hpp"

namespace Tactile {

MoveLayerUpCmd::MoveLayerUpCmd(RegistryRef registry, const LayerID id)
    : ACommand{"Move Layer Up"}
    , mRegistry{registry}
    , mLayerId{id}
{}

void MoveLayerUpCmd::Undo()
{
  Sys::MoveLayerDown(mRegistry, mLayerId);
}

void MoveLayerUpCmd::Redo()
{
  Sys::MoveLayerUp(mRegistry, mLayerId);
}

}  // namespace Tactile
