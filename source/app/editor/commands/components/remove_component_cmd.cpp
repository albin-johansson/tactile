#include "remove_component_cmd.hpp"

namespace tactile {

remove_component_cmd::remove_component_cmd(registry_ref registry,
                                           const context_id contextId,
                                           const component_id componentId)
    : command_base{"Remove Component"}
    , mRegistry{registry}
    , mContextId{contextId}
    , mComponentId{componentId}
{}

void remove_component_cmd::undo()
{
  auto& registry = mRegistry.get();
  sys::restore_component(registry, mSnapshot.value());
  mSnapshot.reset();
}

void remove_component_cmd::redo()
{
  auto& registry = mRegistry.get();
  mSnapshot = sys::remove_component(registry, mContextId, mComponentId);
}

}  // namespace tactile