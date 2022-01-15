#include "rename_property_cmd.hpp"

#include <utility>  // move

#include "core/systems/property_system.hpp"

namespace Tactile {

RenamePropertyCmd::RenamePropertyCmd(RegistryRef registry,
                                     std::string oldName,
                                     std::string newName)
    : ACommand{"Rename Property"}
    , mRegistry{registry}
    , mContextId{Sys::GetCurrentContextId(mRegistry)}
    , mOldName{std::move(oldName)}
    , mNewName{std::move(newName)}
{}

void RenamePropertyCmd::Undo()
{
  auto& context = Sys::GetContext(mRegistry, mContextId);
  Sys::RenameProperty(mRegistry, context, mNewName, mOldName);
}

void RenamePropertyCmd::Redo()
{
  auto& context = Sys::GetContext(mRegistry, mContextId);
  Sys::RenameProperty(mRegistry, context, mOldName, mNewName);
}

}  // namespace Tactile
