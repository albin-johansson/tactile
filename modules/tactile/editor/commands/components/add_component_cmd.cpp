#include "add_component_cmd.hpp"

#include "core/systems/component_system.hpp"

namespace Tactile {

AddComponentCmd::AddComponentCmd(RegistryRef registry,
                                 const ContextID contextId,
                                 const ComponentID componentId)
    : ACommand{"Add Component"}
    , mRegistry{registry}
    , mContextId{contextId}
    , mComponentId{componentId}
{}

void AddComponentCmd::Undo()
{
  auto& registry = mRegistry.get();
  Sys::RemoveComponent(registry, mContextId, mComponentId);
}

void AddComponentCmd::Redo()
{
  auto& registry = mRegistry.get();
  Sys::AddComponent(registry, mContextId, mComponentId);
}

}  // namespace Tactile