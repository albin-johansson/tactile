#include "reset_component_cmd.hpp"

namespace tactile {

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
    sys::update_component(registry, mContextId, mComponentId, name, value);
  }

  mSnapshot.reset();
}

void ResetComponentCmd::Redo()
{
  auto& registry = mRegistry.get();
  mSnapshot = sys::reset_component(registry, mContextId, mComponentId);
}

}  // namespace tactile