#include "rename_component_cmd.hpp"

#include <utility>  // move

#include "core/systems/component_system.hpp"

namespace tactile {

RenameComponentCmd::RenameComponentCmd(RegistryRef registry,
                                       const ComponentID id,
                                       std::string name)
    : ACommand{"Rename Component"}
    , mRegistry{registry}
    , mComponentId{id}
    , mUpdatedName{std::move(name)}
{}

void RenameComponentCmd::Undo()
{
  auto& registry = mRegistry.get();
  sys::RenameComponentDef(registry, mComponentId, mPreviousName.value());
}

void RenameComponentCmd::Redo()
{
  auto& registry = mRegistry.get();

  mPreviousName = sys::GetComponentDefName(registry, mComponentId);
  sys::RenameComponentDef(registry, mComponentId, mUpdatedName);
}

}  // namespace tactile