#include "rename_property_cmd.hpp"

#include <utility>  // move

#include "core/systems/context_system.hpp"
#include "core/systems/property_system.hpp"

namespace tactile {

rename_property_cmd::rename_property_cmd(registry_ref registry,
                                         std::string oldName,
                                         std::string newName)
    : command_base{"Rename Property"}
    , mRegistry{registry}
    , mContextId{sys::current_context_id(mRegistry)}
    , mOldName{std::move(oldName)}
    , mNewName{std::move(newName)}
{}

void rename_property_cmd::undo()
{
  auto& context = sys::get_context(mRegistry, mContextId);
  sys::rename_property(mRegistry, context, mNewName, mOldName);
}

void rename_property_cmd::redo()
{
  auto& context = sys::get_context(mRegistry, mContextId);
  sys::rename_property(mRegistry, context, mOldName, mNewName);
}

}  // namespace tactile
