#include "add_component_cmd.hpp"

#include "core/systems/component_system.hpp"

namespace tactile {

AddComponentCmd::AddComponentCmd(registry_ref registry,
                                 const context_id contextId,
                                 const component_id componentId)
    : command_base{"Add Component"}
    , mRegistry{registry}
    , mContextId{contextId}
    , mComponentId{componentId}
{}

void AddComponentCmd::undo()
{
  auto& registry = mRegistry.get();
  sys::remove_component(registry, mContextId, mComponentId);
}

void AddComponentCmd::redo()
{
  auto& registry = mRegistry.get();
  sys::add_component(registry, mContextId, mComponentId);
}

}  // namespace tactile