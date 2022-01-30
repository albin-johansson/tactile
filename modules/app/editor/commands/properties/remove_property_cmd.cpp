#include "remove_property_cmd.hpp"

#include <utility>  // move

#include "core/systems/property_system.hpp"

namespace tactile {

RemovePropertyCmd::RemovePropertyCmd(RegistryRef registry, std::string name)
    : ACommand{"Remove Property"}
    , mRegistry{registry}
    , mContextId{sys::GetCurrentContextId(mRegistry)}
    , mName{std::move(name)}
{}

void RemovePropertyCmd::Undo()
{
  auto& context = sys::GetContext(mRegistry, mContextId);
  sys::AddProperty(mRegistry, context, mName, mPreviousValue.value());
  mPreviousValue.reset();
}

void RemovePropertyCmd::Redo()
{
  auto& context = sys::GetContext(mRegistry, mContextId);
  mPreviousValue = sys::GetProperty(mRegistry, context, mName).value;
  sys::RemoveProperty(mRegistry, context, mName);
}

}  // namespace tactile
