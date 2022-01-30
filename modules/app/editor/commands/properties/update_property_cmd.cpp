#include "update_property_cmd.hpp"

#include <utility>  // move

#include "core/systems/property_system.hpp"

namespace tactile {

UpdatePropertyCmd::UpdatePropertyCmd(RegistryRef registry,
                                     std::string name,
                                     attribute_value value)
    : ACommand{"Update Property"}
    , mRegistry{registry}
    , mContextId{sys::GetCurrentContextId(mRegistry)}
    , mName{std::move(name)}
    , mNewValue{std::move(value)}
{}

void UpdatePropertyCmd::Undo()
{
  auto& context = sys::GetContext(mRegistry, mContextId);
  sys::UpdateProperty(mRegistry, context, mName, mOldValue.value());
  mOldValue.reset();
}

void UpdatePropertyCmd::Redo()
{
  auto& context = sys::GetContext(mRegistry, mContextId);
  mOldValue = sys::GetProperty(mRegistry, context, mName).value;
  sys::UpdateProperty(mRegistry, context, mName, mNewValue);
}

auto UpdatePropertyCmd::MergeWith(const ACommand& cmd) -> bool
{
  if (GetId() == cmd.GetId()) {
    const auto& other = dynamic_cast<const UpdatePropertyCmd&>(cmd);
    if (mContextId == other.mContextId && mName == other.mName) {
      mNewValue = other.mNewValue;
      return true;
    }
  }

  return false;
}

}  // namespace tactile
