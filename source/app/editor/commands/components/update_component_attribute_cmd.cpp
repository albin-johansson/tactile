#include "update_component_attribute_cmd.hpp"

#include <utility>  // move

#include "core/systems/component_system.hpp"

namespace tactile {

UpdateComponentAttributeCmd::UpdateComponentAttributeCmd(registry_ref registry,
                                                         const component_id id,
                                                         std::string attribute,
                                                         attribute_value value)
    : command_base{"Update Component Attribute"}
    , mRegistry{registry}
    , mComponentId{id}
    , mAttributeName{std::move(attribute)}
    , mUpdatedValue{std::move(value)}
{}

void UpdateComponentAttributeCmd::undo()
{
  auto& registry = mRegistry.get();
  sys::set_component_attribute_value(registry,
                                     mComponentId,
                                     mAttributeName,
                                     mPreviousValue.value());
}

void UpdateComponentAttributeCmd::redo()
{
  auto& registry = mRegistry.get();

  mPreviousValue =
      sys::get_component_attribute_value(registry, mComponentId, mAttributeName);
  sys::set_component_attribute_value(registry,
                                     mComponentId,
                                     mAttributeName,
                                     mUpdatedValue);
}

auto UpdateComponentAttributeCmd::merge_with(const command_base& cmd) -> bool
{
  if (id() == cmd.id()) {
    const auto& other = dynamic_cast<const UpdateComponentAttributeCmd&>(cmd);
    if (mComponentId == other.mComponentId && mAttributeName == other.mAttributeName) {
      mUpdatedValue = other.mUpdatedValue;
      return true;
    }
  }

  return false;
}

}  // namespace tactile