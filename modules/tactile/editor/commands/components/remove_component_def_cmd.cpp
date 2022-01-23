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
  Sys::RestoreComponentDef(registry, mSnapshot.value());
  mSnapshot.reset();
}

void RemoveComponentDefCmd::Redo()
{
  auto& registry = mRegistry.get();
  mSnapshot = Sys::RemoveComponentDef(registry, mComponentId);
}

}  // namespace tactile