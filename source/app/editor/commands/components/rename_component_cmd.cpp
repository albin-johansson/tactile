#include "rename_component_cmd.hpp"

#include <utility>  // move

#include "core/systems/component_system.hpp"

namespace tactile {

RenameComponentCmd::RenameComponentCmd(registry_ref registry,
                                       const component_id id,
                                       std::string name)
    : command_base{"Rename Component"}
    , mRegistry{registry}
    , mComponentId{id}
    , mUpdatedName{std::move(name)}
{}

void RenameComponentCmd::undo()
{
  auto& registry = mRegistry.get();
  sys::rename_component_def(registry, mComponentId, mPreviousName.value());
}

void RenameComponentCmd::redo()
{
  auto& registry = mRegistry.get();

  mPreviousName = sys::get_component_def_name(registry, mComponentId);
  sys::rename_component_def(registry, mComponentId, mUpdatedName);
}

}  // namespace tactile