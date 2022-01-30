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
  sys::rename_component_def(registry, mComponentId, mPreviousName.value());
}

void RenameComponentCmd::Redo()
{
  auto& registry = mRegistry.get();

  mPreviousName = sys::get_component_def_name(registry, mComponentId);
  sys::rename_component_def(registry, mComponentId, mUpdatedName);
}

}  // namespace tactile