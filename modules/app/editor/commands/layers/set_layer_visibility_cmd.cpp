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
  auto& registry = mRegistry.get();

  const auto [entity, layer] = sys::get_layer(registry, mLayerId);
  layer.visible = mPreviousVisibility.value();

  mPreviousVisibility.reset();
}

void SetLayerVisibilityCmd::Redo()
{
  auto& registry = mRegistry.get();

  auto [entity, layer] = sys::get_layer(registry, mLayerId);
  mPreviousVisibility = layer.visible;

  layer.visible = mVisible;
}

}  // namespace tactile
