#include "add_layer_cmd.hpp"

#include "assert.hpp"
#include "core/systems/layers/layer_system.hpp"

namespace tactile {

AddLayerCmd::AddLayerCmd(RegistryRef registry, const LayerType type)
    : ACommand{"Add Layer"}
    , mRegistry{registry}
    , mLayerType{type}
{}

void AddLayerCmd::Undo()
{
  auto& registry = mRegistry.get();

  const auto id = mLayerId.value();
  const auto entity = sys::find_layer(registry, id);
  TACTILE_ASSERT(entity != entt::null);

  mLayerSnapshot = sys::copy_layer(registry, entity);
  sys::remove_layer(registry, entity);
}

void AddLayerCmd::Redo()
{
  if (mLayerSnapshot) {
    sys::restore_layer(mRegistry, *mLayerSnapshot);
  }
  else {
    auto& registry = mRegistry.get();

    entt::entity entity{entt::null};
    switch (mLayerType) {
      case LayerType::TileLayer: {
        entity = sys::make_tile_layer(registry);
        break;
      }
      case LayerType::ObjectLayer: {
        entity = sys::make_object_layer(registry);
        break;
      }
      case LayerType::GroupLayer: {
        entity = sys::make_group_layer(registry);
        break;
      }
    }

    TACTILE_ASSERT(entity != entt::null);
    mLayerId = registry.get<Layer>(entity).id;

    sys::sort_layers(registry);
  }
}

}  // namespace tactile
