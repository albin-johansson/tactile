#include "change_property_type_cmd.hpp"

#include <utility>  // move

#include "core/systems/context_system.hpp"
#include "core/systems/property_system.hpp"

namespace tactile {

ChangePropertyTypeCmd::ChangePropertyTypeCmd(registry_ref registry,
                                             std::string name,
                                             attribute_type type)
    : command_base{"Change Property Type"}
    , mRegistry{registry}
    , mContextId{sys::current_context_id(mRegistry)}
    , mName{std::move(name)}
    , mPropertyType{type}
{}

void ChangePropertyTypeCmd::undo()
{
  const auto& value = mPreviousValue.value();
  const auto type = value.type();

  auto& context = sys::get_context(mRegistry, mContextId);
  sys::change_property_type(mRegistry, context, mName, type);
  sys::update_property(mRegistry, context, mName, value);

  mPreviousValue.reset();
}

void ChangePropertyTypeCmd::redo()
{
  auto& context = sys::get_context(mRegistry, mContextId);
  mPreviousValue = sys::get_property(mRegistry, context, mName).value;
  sys::change_property_type(mRegistry, context, mName, mPropertyType);
}

}  // namespace tactile
