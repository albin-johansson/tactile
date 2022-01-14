#include "remove_component_cmd.hpp"

namespace Tactile {

RemoveComponentCmd::RemoveComponentCmd(RegistryRef registry,
                                       const ContextID contextId,
                                       const ComponentID componentId)
    : ACommand{"Remove Component"}
    , mRegistry{registry}
    , mContextId{contextId}
    , mComponentId{componentId}
{}

void RemoveComponentCmd::Undo()
{
  auto& registry = mRegistry.get();
  Sys::RestoreComponent(registry, mSnapshot.value());
  mSnapshot.reset();
}

void RemoveComponentCmd::Redo()
{
  auto& registry = mRegistry.get();
  mSnapshot = Sys::RemoveComponent(registry, mContextId, mComponentId);
}

}  // namespace Tactile