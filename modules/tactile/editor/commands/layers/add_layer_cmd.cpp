#include "add_layer_cmd.hpp"

#include "assert.hpp"
#include "core/systems/layers/layer_system.hpp"

namespace Tactile {

AddLayerCmd::AddLayerCmd(RegistryRef registry, const LayerType type)
    : ACommand{"Add Layer"}
    , mRegistry{registry}
    , mLayerType{type}
{}

void AddLayerCmd::Undo()
{
  auto& registry = mRegistry.get();

  const auto id = mLayerId.value();
  const auto entity = Sys::FindLayer(registry, id);
  TACTILE_ASSERT(entity != entt::null);

  mLayerSnapshot = Sys::CopyLayer(registry, entity);
  Sys::RemoveLayer(registry, id);
}

void AddLayerCmd::Redo()
{
  if (mLayerSnapshot) {
    Sys::RestoreLayer(mRegistry, *mLayerSnapshot);
  }
  else {
    auto& registry = mRegistry.get();

    entt::entity entity{entt::null};
    switch (mLayerType) {
      case LayerType::TileLayer: {
        entity = Sys::AddTileLayer(registry);
        break;
      }
      case LayerType::ObjectLayer: {
        entity = Sys::AddObjectLayer(registry);
        break;
      }
      case LayerType::GroupLayer: {
        entity = Sys::AddGroupLayer(registry);
        break;
      }
    }

    TACTILE_ASSERT(entity != entt::null);
    mLayerId = registry.get<Layer>(entity).id;

    Sys::SortLayers(registry);
  }
}

}  // namespace Tactile
