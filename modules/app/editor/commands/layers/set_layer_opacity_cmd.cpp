#include "set_layer_opacity_cmd.hpp"

#include "core/systems/layers/layer_system.hpp"

namespace tactile {

SetLayerOpacityCmd::SetLayerOpacityCmd(RegistryRef registry,
                                       const layer_id id,
                                       const float opacity)
    : ACommand{"Set Layer Opacity"}
    , mRegistry{registry}
    , mLayerId{id}
    , mOpacity{opacity}
{}

void SetLayerOpacityCmd::Undo()
{
  auto& registry = mRegistry.get();

  auto&& [entity, layer] = sys::get_layer(registry, mLayerId);
  layer.opacity = mPreviousOpacity.value();

  mPreviousOpacity.reset();
}

void SetLayerOpacityCmd::Redo()
{
  auto& registry = mRegistry.get();

  auto&& [entity, layer] = sys::get_layer(registry, mLayerId);
  mPreviousOpacity = layer.opacity;

  layer.opacity = mOpacity;
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

}  // namespace tactile
