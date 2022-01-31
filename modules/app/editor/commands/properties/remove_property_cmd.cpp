#include "remove_property_cmd.hpp"

#include <utility>  // move

#include "core/systems/property_system.hpp"

namespace tactile {

RemovePropertyCmd::RemovePropertyCmd(registry_ref registry, std::string name)
    : command_base{"Remove Property"}
    , mRegistry{registry}
    , mContextId{sys::GetCurrentContextId(mRegistry)}
    , mName{std::move(name)}
{}

void RemovePropertyCmd::undo()
{
  auto& context = sys::GetContext(mRegistry, mContextId);
  sys::AddProperty(mRegistry, context, mName, mPreviousValue.value());
  mPreviousValue.reset();
}

void RemovePropertyCmd::redo()
{
  auto& context = sys::GetContext(mRegistry, mContextId);
  mPreviousValue = sys::GetProperty(mRegistry, context, mName).value;
  sys::RemoveProperty(mRegistry, context, mName);
}

}  // namespace tactile
