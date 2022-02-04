#include "set_layer_visibility_cmd.hpp"

#include "core/systems/layers/layer_system.hpp"

namespace tactile {

SetLayerVisibilityCmd::SetLayerVisibilityCmd(registry_ref registry,
                                             const layer_id id,
                                             const bool visible)
    : command_base{"Set Layer Visibility"}
    , mRegistry{registry}
    , mLayerId{id}
    , mVisible{visible}
{}

void SetLayerVisibilityCmd::undo()
{
  auto& registry = mRegistry.get();

  const auto [entity, layer] = sys::get_layer(registry, mLayerId);
  layer.visible = mPreviousVisibility.value();

  mPreviousVisibility.reset();
}

void SetLayerVisibilityCmd::redo()
{
  auto& registry = mRegistry.get();

  auto [entity, layer] = sys::get_layer(registry, mLayerId);
  mPreviousVisibility = layer.visible;

  layer.visible = mVisible;
}

}  // namespace tactile
