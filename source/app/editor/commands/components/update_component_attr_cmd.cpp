#include "update_component_attr_cmd.hpp"

#include <utility>  // move

#include "core/systems/component_system.hpp"

namespace tactile {

update_component_attr_cmd::update_component_attr_cmd(registry_ref registry,
                                                     const component_id id,
                                                     std::string attribute,
                                                     attribute_value value)
    : command_base{"update Component Attribute"}
    , mRegistry{registry}
    , mComponentId{id}
    , mAttributeName{std::move(attribute)}
    , mUpdatedValue{std::move(value)}
{}

void update_component_attr_cmd::undo()
{
  auto& registry = mRegistry.get();
  sys::set_component_attribute_value(registry,
                                     mComponentId,
                                     mAttributeName,
                                     mPreviousValue.value());
}

void update_component_attr_cmd::redo()
{
  auto& registry = mRegistry.get();

  mPreviousValue =
      sys::get_component_attribute_value(registry, mComponentId, mAttributeName);
  sys::set_component_attribute_value(registry,
                                     mComponentId,
                                     mAttributeName,
                                     mUpdatedValue);
}

auto update_component_attr_cmd::merge_with(const command_base& cmd) -> bool
{
  if (id() == cmd.id()) {
    const auto& other = dynamic_cast<const update_component_attr_cmd&>(cmd);
    if (mComponentId == other.mComponentId && mAttributeName == other.mAttributeName) {
      mUpdatedValue = other.mUpdatedValue;
      return true;
    }
  }

  return false;
}

}  // namespace tactile