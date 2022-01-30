#include "remove_component_cmd.hpp"

namespace tactile {

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
  sys::restore_component(registry, mSnapshot.value());
  mSnapshot.reset();
}

void RemoveComponentCmd::Redo()
{
  auto& registry = mRegistry.get();
  mSnapshot = sys::remove_component(registry, mContextId, mComponentId);
}

}  // namespace tactile