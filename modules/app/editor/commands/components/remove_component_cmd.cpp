#include "remove_component_cmd.hpp"

namespace tactile {

RemoveComponentCmd::RemoveComponentCmd(RegistryRef registry,
                                       const context_id contextId,
                                       const component_id componentId)
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