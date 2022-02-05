#include "update_property_cmd.hpp"

#include <utility>  // move

#include "core/systems/context_system.hpp"
#include "core/systems/property_system.hpp"

namespace tactile {

UpdatePropertyCmd::UpdatePropertyCmd(registry_ref registry,
                                     std::string name,
                                     attribute_value value)
    : command_base{"Update Property"}
    , mRegistry{registry}
    , mContextId{sys::current_context_id(mRegistry)}
    , mName{std::move(name)}
    , mNewValue{std::move(value)}
{}

void UpdatePropertyCmd::undo()
{
  auto& context = sys::get_context(mRegistry, mContextId);
  sys::update_property(mRegistry, context, mName, mOldValue.value());
  mOldValue.reset();
}

void UpdatePropertyCmd::redo()
{
  auto& context = sys::get_context(mRegistry, mContextId);
  mOldValue = sys::get_property(mRegistry, context, mName).value;
  sys::update_property(mRegistry, context, mName, mNewValue);
}

auto UpdatePropertyCmd::merge_with(const command_base& cmd) -> bool
{
  if (id() == cmd.id()) {
    const auto& other = dynamic_cast<const UpdatePropertyCmd&>(cmd);
    if (mContextId == other.mContextId && mName == other.mName) {
      mNewValue = other.mNewValue;
      return true;
    }
  }

  return false;
}

}  // namespace tactile