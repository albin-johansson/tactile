#include "reset_component_cmd.hpp"

namespace tactile {

reset_component_cmd::reset_component_cmd(registry_ref registry,
                                         const context_id contextId,
                                         const component_id componentId)
    : command_base{"Reset Component Values"}
    , mRegistry{registry}
    , mContextId{contextId}
    , mComponentId{componentId}
{}

void reset_component_cmd::undo()
{
  auto& registry = mRegistry.get();
  const auto& snapshot = mSnapshot.value();

  for (const auto& [name, value] : snapshot.values) {
    sys::update_component(registry, mContextId, mComponentId, name, value);
  }

  mSnapshot.reset();
}

void reset_component_cmd::redo()
{
  auto& registry = mRegistry.get();
  mSnapshot = sys::reset_component(registry, mContextId, mComponentId);
}

}  // namespace tactile