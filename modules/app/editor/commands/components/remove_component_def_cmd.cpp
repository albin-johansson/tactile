#include "remove_component_def_cmd.hpp"

namespace tactile {

RemoveComponentDefCmd::RemoveComponentDefCmd(RegistryRef registry, const ComponentID id)
    : ACommand{"Remove Component Definition"}
    , mRegistry{registry}
    , mComponentId{id}
{}

void RemoveComponentDefCmd::Undo()
{
  auto& registry = mRegistry.get();
  sys::restore_component_def(registry, mSnapshot.value());
  mSnapshot.reset();
}

void RemoveComponentDefCmd::Redo()
{
  auto& registry = mRegistry.get();
  mSnapshot = sys::remove_component_def(registry, mComponentId);
}

}  // namespace tactile