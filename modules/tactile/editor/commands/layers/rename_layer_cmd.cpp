#include "rename_layer_cmd.hpp"

#include <utility>  // move

#include "assert.hpp"
#include "core/components/property_context.hpp"
#include "core/systems/layers/layer_system.hpp"

namespace tactile {

RenameLayerCmd::RenameLayerCmd(RegistryRef registry, const LayerID id, std::string name)
    : ACommand{"Rename Layer"}
    , mRegistry{registry}
    , mLayerId{id}
    , mName{std::move(name)}
{}

void RenameLayerCmd::Undo()
{
  auto& registry = mRegistry.get();

  const auto entity = Sys::FindLayer(registry, mLayerId);
  TACTILE_ASSERT(entity != entt::null);

  auto& context = registry.get<PropertyContext>(entity);
  context.name = mPreviousName.value();

  mPreviousName.reset();
}

void RenameLayerCmd::Redo()
{
  auto& registry = mRegistry.get();

  const auto entity = Sys::FindLayer(registry, mLayerId);
  TACTILE_ASSERT(entity != entt::null);

  auto& context = registry.get<PropertyContext>(entity);
  mPreviousName = context.name;
  context.name = mName;
}

}  // namespace tactile
