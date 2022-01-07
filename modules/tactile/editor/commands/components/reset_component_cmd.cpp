#include "reset_component_cmd.hpp"

namespace Tactile {

ResetComponentCmd::ResetComponentCmd(RegistryRef registry,
                                     const ContextID contextId,
                                     const ComponentID componentId)
    : ACommand{"Reset Component Values"}
    , mRegistry{registry}
    , mContextId{contextId}
    , mComponentId{componentId}
{}

void ResetComponentCmd::Undo()
{
  auto& registry = mRegistry.get();
  const auto& snapshot = mSnapshot.value();

  for (const auto& [name, value] : snapshot.values) {
    Sys::UpdateComponent(registry, mContextId, mComponentId, name, value);
  }

  mSnapshot.reset();
}

void ResetComponentCmd::Redo()
{
  auto& registry = mRegistry.get();
  mSnapshot = Sys::ResetComponent(registry, mContextId, mComponentId);
}

}  // namespace Tactile