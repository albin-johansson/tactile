#include "add_component_cmd.hpp"

#include "core/systems/component_system.hpp"

namespace tactile {

add_component_cmd::add_component_cmd(registry_ref registry,
                                     const context_id contextId,
                                     const component_id componentId)
    : command_base{"Add Component"}
    , mRegistry{registry}
    , mContextId{contextId}
    , mComponentId{componentId}
{}

void add_component_cmd::undo()
{
  auto& registry = mRegistry.get();
  sys::remove_component(registry, mContextId, mComponentId);
}

void add_component_cmd::redo()
{
  auto& registry = mRegistry.get();
  sys::add_component(registry, mContextId, mComponentId);
}

}  // namespace tactile