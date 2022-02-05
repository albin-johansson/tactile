#include "rename_layer_cmd.hpp"

#include <utility>  // move

#include "core/components/attribute_context.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "misc/assert.hpp"

namespace tactile {

rename_layer_cmd::rename_layer_cmd(registry_ref registry,
                                   const layer_id id,
                                   std::string name)
    : command_base{"Rename Layer"}
    , mRegistry{registry}
    , mLayerId{id}
    , mName{std::move(name)}
{}

void rename_layer_cmd::undo()
{
  auto& registry = mRegistry.get();

  const auto entity = sys::find_layer(registry, mLayerId);
  TACTILE_ASSERT(entity != entt::null);

  auto& context = registry.get<comp::attribute_context>(entity);
  context.name = mPreviousName.value();

  mPreviousName.reset();
}

void rename_layer_cmd::redo()
{
  auto& registry = mRegistry.get();

  const auto entity = sys::find_layer(registry, mLayerId);
  TACTILE_ASSERT(entity != entt::null);

  auto& context = registry.get<comp::attribute_context>(entity);
  mPreviousName = context.name;
  context.name = mName;
}

}  // namespace tactile
