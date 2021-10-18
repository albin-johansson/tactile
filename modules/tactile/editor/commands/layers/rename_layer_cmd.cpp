#include "rename_layer_cmd.hpp"

#include <cassert>  // assert
#include <utility>  // move

#include "tactile/core/components/property_context.hpp"
#include "tactile/core/systems/layers/layer_system.hpp"

namespace Tactile {

RenameLayerCmd::RenameLayerCmd(Ref<entt::registry> registry,
                               const LayerID id,
                               std::string name)
    : ACommand{"Rename Layer"}
    , mRegistry{registry}
    , mLayerId{id}
    , mName{std::move(name)}
{}

void RenameLayerCmd::Undo()
{
  auto& registry = mRegistry.get();

  const auto entity = Sys::FindLayer(registry, mLayerId);
  assert(entity != entt::null);

  auto& context = registry.get<PropertyContext>(entity);
  context.name = mPreviousName.value();

  mPreviousName.reset();
}

void RenameLayerCmd::Redo()
{
  auto& registry = mRegistry.get();

  const auto entity = Sys::FindLayer(registry, mLayerId);
  assert(entity != entt::null);

  auto& context = registry.get<PropertyContext>(entity);
  mPreviousName = context.name;
  context.name = mName;
}

}  // namespace Tactile
