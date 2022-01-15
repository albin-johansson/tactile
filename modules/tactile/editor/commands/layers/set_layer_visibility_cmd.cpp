#include "set_layer_visibility_cmd.hpp"

#include "core/systems/layers/layer_system.hpp"

namespace Tactile {

SetLayerVisibilityCmd::SetLayerVisibilityCmd(RegistryRef registry,
                                             const LayerID id,
                                             const bool visible)
    : ACommand{"Set Layer Visibility"}
    , mRegistry{registry}
    , mLayerId{id}
    , mVisible{visible}
{}

void SetLayerVisibilityCmd::Undo()
{
  Sys::SetLayerVisible(mRegistry, mLayerId, mPreviousVisibility.value());
  mPreviousVisibility.reset();
}

void SetLayerVisibilityCmd::Redo()
{
  mPreviousVisibility = Sys::IsLayerVisible(mRegistry, mLayerId);
  Sys::SetLayerVisible(mRegistry, mLayerId, mVisible);
}

}  // namespace Tactile
