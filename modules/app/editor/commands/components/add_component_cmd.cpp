#include "add_component_cmd.hpp"

#include "core/systems/component_system.hpp"

namespace tactile {

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
  sys::remove_component(registry, mContextId, mComponentId);
}

void AddComponentCmd::Redo()
{
  auto& registry = mRegistry.get();
  sys::add_component(registry, mContextId, mComponentId);
}

}  // namespace tactile