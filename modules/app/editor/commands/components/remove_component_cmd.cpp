#include "remove_component_cmd.hpp"

namespace tactile {

RemoveComponentCmd::RemoveComponentCmd(registry_ref registry,
                                       const context_id contextId,
                                       const component_id componentId)
    : command_base{"Remove Component"}
    , mRegistry{registry}
    , mContextId{contextId}
    , mComponentId{componentId}
{}

void RemoveComponentCmd::undo()
{
  auto& registry = mRegistry.get();
  sys::restore_component(registry, mSnapshot.value());
  mSnapshot.reset();
}

void RemoveComponentCmd::redo()
{
  auto& registry = mRegistry.get();
  mSnapshot = sys::remove_component(registry, mContextId, mComponentId);
}

}  // namespace tactile