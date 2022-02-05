#include "remove_property_cmd.hpp"

#include <utility>  // move

#include "core/systems/context_system.hpp"
#include "core/systems/property_system.hpp"

namespace tactile {

remove_property_cmd::remove_property_cmd(registry_ref registry, std::string name)
    : command_base{"Remove Property"}
    , mRegistry{registry}
    , mContextId{sys::current_context_id(mRegistry)}
    , mName{std::move(name)}
{}

void remove_property_cmd::undo()
{
  auto& context = sys::get_context(mRegistry, mContextId);
  sys::add_property(mRegistry, context, mName, mPreviousValue.value());
  mPreviousValue.reset();
}

void remove_property_cmd::redo()
{
  auto& context = sys::get_context(mRegistry, mContextId);
  mPreviousValue = sys::get_property(mRegistry, context, mName).value;
  sys::remove_property(mRegistry, context, mName);
}

}  // namespace tactile
