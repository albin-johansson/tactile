#include "rename_property_cmd.hpp"

#include <utility>  // move

#include "core/systems/context_system.hpp"
#include "core/systems/property_system.hpp"

namespace tactile {

RenamePropertyCmd::RenamePropertyCmd(registry_ref registry,
                                     std::string oldName,
                                     std::string newName)
    : command_base{"Rename Property"}
    , mRegistry{registry}
    , mContextId{sys::current_context_id(mRegistry)}
    , mOldName{std::move(oldName)}
    , mNewName{std::move(newName)}
{}

void RenamePropertyCmd::undo()
{
  auto& context = sys::get_context(mRegistry, mContextId);
  sys::rename_property(mRegistry, context, mNewName, mOldName);
}

void RenamePropertyCmd::redo()
{
  auto& context = sys::get_context(mRegistry, mContextId);
  sys::rename_property(mRegistry, context, mOldName, mNewName);
}

}  // namespace tactile
