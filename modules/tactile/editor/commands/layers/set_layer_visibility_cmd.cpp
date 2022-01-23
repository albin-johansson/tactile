#include "set_layer_visibility_cmd.hpp"

#include "core/systems/layers/layer_system.hpp"

namespace tactile {

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
  sys::SetLayerVisible(mRegistry, mLayerId, mPreviousVisibility.value());
  mPreviousVisibility.reset();
}

void SetLayerVisibilityCmd::Redo()
{
  mPreviousVisibility = sys::IsLayerVisible(mRegistry, mLayerId);
  sys::SetLayerVisible(mRegistry, mLayerId, mVisible);
}

}  // namespace tactile
