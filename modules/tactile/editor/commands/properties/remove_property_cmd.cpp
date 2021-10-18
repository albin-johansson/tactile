#include "remove_property_cmd.hpp"

#include <utility>  // move

#include "tactile/core/systems/property_system.hpp"

namespace Tactile {

RemovePropertyCmd::RemovePropertyCmd(Ref<entt::registry> registry, std::string name)
    : ACommand{"Remove Property"}
    , mRegistry{registry}
    , mContextId{Sys::GetCurrentContext(mRegistry).id}
    , mName{std::move(name)}
{}

void RemovePropertyCmd::Undo()
{
  Sys::AddProperty(mRegistry, mContextId, mName, mPreviousValue.value());
  mPreviousValue.reset();
}

void RemovePropertyCmd::Redo()
{
  mPreviousValue = Sys::GetPropertyValue(mRegistry, mContextId, mName);
  Sys::RemoveProperty(mRegistry, mContextId, mName);
}

}  // namespace Tactile
