#include "rename_property_cmd.hpp"

#include <utility>  // move

#include "tactile/core/systems/property_system.hpp"

namespace Tactile {

RenamePropertyCmd::RenamePropertyCmd(Ref<entt::registry> registry,
                                     std::string oldName,
                                     std::string newName)
    : ACommand{"Rename Property"}
    , mRegistry{registry}
    , mContextId{Sys::GetCurrentContext(mRegistry).id}
    , mOldName{std::move(oldName)}
    , mNewName{std::move(newName)}
{}

void RenamePropertyCmd::Undo()
{
  Sys::RenameProperty(mRegistry, mContextId, mNewName, mOldName);
}

void RenamePropertyCmd::Redo()
{
  Sys::RenameProperty(mRegistry, mContextId, mOldName, mNewName);
}

}  // namespace Tactile
