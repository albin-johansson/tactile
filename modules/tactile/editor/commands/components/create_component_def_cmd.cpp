#include "create_component_def_cmd.hpp"

#include <utility>  // move

#include "core/systems/component_system.hpp"

namespace Tactile {

CreateComponentDefCmd::CreateComponentDefCmd(Ref<entt::registry> registry,
                                             std::string name)
    : ACommand{"Create Component Definition"}
    , mRegistry{registry}
    , mName{std::move(name)}
{}

void CreateComponentDefCmd::Undo()
{
  auto& registry = mRegistry.get();
  Sys::RemoveComponentDef(registry, mComponentId.value());
}

void CreateComponentDefCmd::Redo()
{
  auto& registry = mRegistry.get();
  if (!mComponentId) {
    mComponentId = Sys::CreateComponentDef(registry, mName);
  }
  else {
    Sys::CreateComponentDef(mRegistry, *mComponentId, mName);
  }
}

}  // namespace Tactile
