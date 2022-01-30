#include "rename_property_cmd.hpp"

#include <utility>  // move

#include "core/systems/property_system.hpp"

namespace tactile {

RenamePropertyCmd::RenamePropertyCmd(RegistryRef registry,
                                     std::string oldName,
                                     std::string newName)
    : ACommand{"Rename Property"}
    , mRegistry{registry}
    , mContextId{sys::GetCurrentContextId(mRegistry)}
    , mOldName{std::move(oldName)}
    , mNewName{std::move(newName)}
{}

void RenamePropertyCmd::Undo()
{
  auto& context = sys::GetContext(mRegistry, mContextId);
  sys::RenameProperty(mRegistry, context, mNewName, mOldName);
}

void RenamePropertyCmd::Redo()
{
  auto& context = sys::GetContext(mRegistry, mContextId);
  sys::RenameProperty(mRegistry, context, mOldName, mNewName);
}

}  // namespace tactile
