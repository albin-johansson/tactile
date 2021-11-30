#include "remove_property_cmd.hpp"

#include <utility>  // move

#include "core/systems/property_system.hpp"

namespace Tactile {

RemovePropertyCmd::RemovePropertyCmd(Ref<entt::registry> registry, std::string name)
    : ACommand{"Remove Property"}
    , mRegistry{registry}
    , mContextId{Sys::GetCurrentContextId(mRegistry)}
    , mName{std::move(name)}
{}

void RemovePropertyCmd::Undo()
{
  auto& context = Sys::GetContext(mRegistry, mContextId);
  Sys::AddProperty(mRegistry, context, mName, mPreviousValue.value());
  mPreviousValue.reset();
}

void RemovePropertyCmd::Redo()
{
  auto& context = Sys::GetContext(mRegistry, mContextId);
  mPreviousValue = Sys::GetProperty(mRegistry, context, mName).value;
  Sys::RemoveProperty(mRegistry, context, mName);
}

}  // namespace Tactile
