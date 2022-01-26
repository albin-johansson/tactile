#include "rename_layer_cmd.hpp"

#include <utility>  // move

#include "assert.hpp"
#include "core/components/attribute_context.hpp"
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

  const auto entity = sys::FindLayer(registry, mLayerId);
  TACTILE_ASSERT(entity != entt::null);

  auto& context = registry.get<comp::attribute_context>(entity);
  context.name = mPreviousName.value();

  mPreviousName.reset();
}

void RenameLayerCmd::Redo()
{
  auto& registry = mRegistry.get();

  const auto entity = sys::FindLayer(registry, mLayerId);
  TACTILE_ASSERT(entity != entt::null);

  auto& context = registry.get<comp::attribute_context>(entity);
  mPreviousName = context.name;
  context.name = mName;
}

}  // namespace tactile
