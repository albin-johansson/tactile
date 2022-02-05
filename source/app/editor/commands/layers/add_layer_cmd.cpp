#include "add_layer_cmd.hpp"

#include "core/systems/layers/layer_system.hpp"
#include "core/systems/layers/layer_tree_system.hpp"
#include "misc/assert.hpp"

namespace tactile {

AddLayerCmd::AddLayerCmd(registry_ref registry, const layer_type type)
    : command_base{"Add Layer"}
    , mRegistry{registry}
    , mLayerType{type}
{}

void AddLayerCmd::undo()
{
  auto& registry = mRegistry.get();

  const auto id = mLayerId.value();
  const auto entity = sys::find_layer(registry, id);
  TACTILE_ASSERT(entity != entt::null);

  mLayerSnapshot = sys::remove_layer(registry, entity);
}

void AddLayerCmd::redo()
{
  if (mLayerSnapshot) {
    sys::restore_layer(mRegistry, *mLayerSnapshot);
  }
  else {
    auto& registry = mRegistry.get();

    entt::entity entity{entt::null};
    switch (mLayerType) {
      case layer_type::tile_layer: {
        entity = sys::make_tile_layer(registry);
        break;
      }
      case layer_type::object_layer: {
        entity = sys::make_object_layer(registry);
        break;
      }
      case layer_type::group_layer: {
        entity = sys::make_group_layer(registry);
        break;
      }
    }

    TACTILE_ASSERT(entity != entt::null);
    mLayerId = registry.get<comp::layer>(entity).id;

    sys::sort_layers(registry);
  }
}

}  // namespace tactile
