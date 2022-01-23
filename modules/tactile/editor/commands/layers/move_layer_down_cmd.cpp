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
  Sys::MoveLayerUp(mRegistry, mLayerId);
}

void MoveLayerDownCmd::Redo()
{
  Sys::MoveLayerDown(mRegistry, mLayerId);
}

}  // namespace tactile
