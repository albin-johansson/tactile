#include "update_property_cmd.hpp"

#include <utility>  // move

#include "core/systems/property_system.hpp"

namespace Tactile {

UpdatePropertyCmd::UpdatePropertyCmd(Ref<entt::registry> registry,
                                     std::string name,
                                     PropertyValue value)
    : ACommand{"Update Property"}
    , mRegistry{registry}
    , mContextId{Sys::GetCurrentContext(mRegistry).id}
    , mName{std::move(name)}
    , mNewValue{std::move(value)}
{}

void UpdatePropertyCmd::Undo()
{
  Sys::UpdateProperty(mRegistry, mContextId, mName, mOldValue.value());
  mOldValue.reset();
}

void UpdatePropertyCmd::Redo()
{
  mOldValue = Sys::GetPropertyValue(mRegistry, mContextId, mName);
  Sys::UpdateProperty(mRegistry, mContextId, mName, mNewValue);
}

auto UpdatePropertyCmd::MergeWith(const ACommand& cmd) -> bool
{
  if (GetId() == cmd.GetId())
  {
    const auto& other = dynamic_cast<const UpdatePropertyCmd&>(cmd);
    if (mContextId == other.mContextId && mName == other.mName)
    {
      mNewValue = other.mNewValue;
      return true;
    }
  }

  return false;
}

}  // namespace Tactile
