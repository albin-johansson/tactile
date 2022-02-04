#include "create_component_def_cmd.hpp"

#include <utility>  // move

#include "core/systems/component_system.hpp"

namespace tactile {

CreateComponentDefCmd::CreateComponentDefCmd(registry_ref registry, std::string name)
    : command_base{"Create Component Definition"}
    , mRegistry{registry}
    , mName{std::move(name)}
{}

void CreateComponentDefCmd::undo()
{
  auto& registry = mRegistry.get();
  sys::remove_component_def(registry, mComponentId.value());
}

void CreateComponentDefCmd::redo()
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
