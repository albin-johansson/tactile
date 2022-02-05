#include "remove_component_def_cmd.hpp"

namespace tactile {

remove_component_def_cmd::remove_component_def_cmd(registry_ref registry,
                                                   const component_id id)
    : command_base{"Remove Component Definition"}
    , mRegistry{registry}
    , mComponentId{id}
{}

void remove_component_def_cmd::undo()
{
  auto& registry = mRegistry.get();
  sys::restore_component_def(registry, mSnapshot.value());
  mSnapshot.reset();
}

void remove_component_def_cmd::redo()
{
  auto& registry = mRegistry.get();
  mSnapshot = sys::remove_component_def(registry, mComponentId);
}

}  // namespace tactile