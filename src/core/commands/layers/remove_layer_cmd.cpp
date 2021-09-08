#include "remove_layer_cmd.hpp"

#include <cassert>  // assert

#include "core/systems/layer_system.hpp"

namespace Tactile {

RemoveLayerCmd::RemoveLayerCmd(Ref<entt::registry> registry, const LayerID id)
    : ACommand{"Remove Layer"}
    , mRegistry{registry}
    , mLayerId{id}
{}

void RemoveLayerCmd::Undo()
{
  Sys::RestoreLayer(mRegistry, mLayerSnapshot.value());
}

void RemoveLayerCmd::Redo()
{
  const auto entity = Sys::FindLayer(mRegistry, mLayerId);
  assert(entity != entt::null);

  mLayerSnapshot = Sys::CopyLayer(mRegistry, entity);
  Sys::RemoveLayer(mRegistry, entity);
}

}  // namespace Tactile
