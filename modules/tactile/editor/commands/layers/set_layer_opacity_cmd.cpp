#include "set_layer_opacity_cmd.hpp"

#include "core/systems/layers/layer_system.hpp"

namespace Tactile {

SetLayerOpacityCmd::SetLayerOpacityCmd(Ref<entt::registry> registry,
                                       const LayerID id,
                                       const float opacity)
    : ACommand{"Set Layer Opacity"}
    , mRegistry{registry}
    , mLayerId{id}
    , mOpacity{opacity}
{}

void SetLayerOpacityCmd::Undo()
{
  Sys::SetLayerOpacity(mRegistry, mLayerId, mPreviousOpacity.value());
  mPreviousOpacity.reset();
}

void SetLayerOpacityCmd::Redo()
{
  mPreviousOpacity = Sys::GetLayerOpacity(mRegistry, mLayerId);
  Sys::SetLayerOpacity(mRegistry, mLayerId, mOpacity);
}

auto SetLayerOpacityCmd::MergeWith(const ACommand& cmd) -> bool
{
  if (GetId() == cmd.GetId()) {
    const auto& other = dynamic_cast<const SetLayerOpacityCmd&>(cmd);
    if (mLayerId == other.mLayerId) {
      mOpacity = other.mOpacity;
      return true;
    }
  }

  return false;
}

}  // namespace Tactile
