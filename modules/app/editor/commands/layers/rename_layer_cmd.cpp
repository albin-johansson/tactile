#include "rename_layer_cmd.hpp"

#include <utility>  // move

#include "assert.hpp"
#include "core/components/attribute_context.hpp"
#include "core/systems/layers/layer_system.hpp"

namespace tactile {

RenameLayerCmd::RenameLayerCmd(registry_ref registry, const layer_id id, std::string name)
    : command_base{"Rename Layer"}
    , mRegistry{registry}
    , mLayerId{id}
    , mName{std::move(name)}
{}

void RenameLayerCmd::undo()
{
  auto& registry = mRegistry.get();

  const auto entity = sys::find_layer(registry, mLayerId);
  TACTILE_ASSERT(entity != entt::null);

  auto& context = registry.get<comp::attribute_context>(entity);
  context.name = mPreviousName.value();

  mPreviousName.reset();
}

void RenameLayerCmd::redo()
{
  auto& registry = mRegistry.get();

  const auto entity = sys::find_layer(registry, mLayerId);
  TACTILE_ASSERT(entity != entt::null);

  auto& context = registry.get<comp::attribute_context>(entity);
  mPreviousName = context.name;
  context.name = mName;
}

}  // namespace tactile
