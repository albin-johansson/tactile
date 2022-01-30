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
  sys::remove_component_def(registry, mComponentId.value());
}

void CreateComponentDefCmd::Redo()
{
  auto& registry = mRegistry.get();
  if (!mComponentId) {
    mComponentId = sys::make_component_def(registry, mName);
  }
  else {
    sys::make_component_def(mRegistry, *mComponentId, mName);
  }
}

}  // namespace tactile
