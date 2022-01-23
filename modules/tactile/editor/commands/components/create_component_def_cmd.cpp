#include "create_component_def_cmd.hpp"

#include <utility>  // move

#include "core/systems/component_system.hpp"

namespace tactile {

CreateComponentDefCmd::CreateComponentDefCmd(RegistryRef registry, std::string name)
    : ACommand{"Create Component Definition"}
    , mRegistry{registry}
    , mName{std::move(name)}
{}

void CreateComponentDefCmd::Undo()
{
  auto& registry = mRegistry.get();
  sys::RemoveComponentDef(registry, mComponentId.value());
}

void CreateComponentDefCmd::Redo()
{
  auto& registry = mRegistry.get();
  if (!mComponentId) {
    mComponentId = sys::CreateComponentDef(registry, mName);
  }
  else {
    sys::CreateComponentDef(mRegistry, *mComponentId, mName);
  }
}

}  // namespace tactile
