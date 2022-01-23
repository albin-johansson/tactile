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
  const auto entity = sys::FindLayer(registry, id);
  TACTILE_ASSERT(entity != entt::null);

  mLayerSnapshot = sys::CopyLayer(registry, entity);
  sys::RemoveLayer(registry, id);
}

void AddLayerCmd::Redo()
{
  if (mLayerSnapshot) {
    sys::RestoreLayer(mRegistry, *mLayerSnapshot);
  }
  else {
    auto& registry = mRegistry.get();

    entt::entity entity{entt::null};
    switch (mLayerType) {
      case LayerType::TileLayer: {
        entity = sys::AddTileLayer(registry);
        break;
      }
      case LayerType::ObjectLayer: {
        entity = sys::AddObjectLayer(registry);
        break;
      }
      case LayerType::GroupLayer: {
        entity = sys::AddGroupLayer(registry);
        break;
      }
    }

    TACTILE_ASSERT(entity != entt::null);
    mLayerId = registry.get<Layer>(entity).id;

    sys::SortLayers(registry);
  }
}

}  // namespace tactile
