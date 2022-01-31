#include "set_layer_opacity_cmd.hpp"

#include "core/systems/layers/layer_system.hpp"

namespace tactile {

SetLayerOpacityCmd::SetLayerOpacityCmd(registry_ref registry,
                                       const layer_id id,
                                       const float opacity)
    : command_base{"Set Layer Opacity"}
    , mRegistry{registry}
    , mLayerId{id}
    , mOpacity{opacity}
{}

void SetLayerOpacityCmd::undo()
{
  auto& registry = mRegistry.get();

  auto&& [entity, layer] = sys::get_layer(registry, mLayerId);
  layer.opacity = mPreviousOpacity.value();

  mPreviousOpacity.reset();
}

void SetLayerOpacityCmd::redo()
{
  auto& registry = mRegistry.get();

  auto&& [entity, layer] = sys::get_layer(registry, mLayerId);
  mPreviousOpacity = layer.opacity;

  layer.opacity = mOpacity;
}

auto SetLayerOpacityCmd::merge_with(const command_base& cmd) -> bool
{
  if (id() == cmd.id()) {
    const auto& other = dynamic_cast<const SetLayerOpacityCmd&>(cmd);
    if (mLayerId == other.mLayerId) {
      mOpacity = other.mOpacity;
      return true;
    }
  }

  return false;
}

}  // namespace tactile
