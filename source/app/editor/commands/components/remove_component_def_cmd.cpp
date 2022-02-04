#include "remove_component_def_cmd.hpp"

namespace tactile {

RemoveComponentDefCmd::RemoveComponentDefCmd(registry_ref registry, const component_id id)
    : command_base{"Remove Component Definition"}
    , mRegistry{registry}
    , mComponentId{id}
{}

void RemoveComponentDefCmd::undo()
{
  auto& registry = mRegistry.get();
  sys::restore_component_def(registry, mSnapshot.value());
  mSnapshot.reset();
}

void RemoveComponentDefCmd::redo()
{
  auto& registry = mRegistry.get();
  mSnapshot = sys::remove_component_def(registry, mComponentId);
}

}  // namespace tactile