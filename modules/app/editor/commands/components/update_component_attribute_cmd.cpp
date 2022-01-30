#include "update_component_attribute_cmd.hpp"

#include <utility>  // move

#include "core/systems/component_system.hpp"

namespace tactile {

UpdateComponentAttributeCmd::UpdateComponentAttributeCmd(RegistryRef registry,
                                                         const ComponentID id,
                                                         std::string attribute,
                                                         attribute_value value)
    : ACommand{"Update Component Attribute"}
    , mRegistry{registry}
    , mComponentId{id}
    , mAttributeName{std::move(attribute)}
    , mUpdatedValue{std::move(value)}
{}

void UpdateComponentAttributeCmd::Undo()
{
  auto& registry = mRegistry.get();
  sys::set_component_attribute_value(registry,
                                     mComponentId,
                                     mAttributeName,
                                     mPreviousValue.value());
}

void UpdateComponentAttributeCmd::Redo()
{
  auto& registry = mRegistry.get();

  mPreviousValue =
      sys::get_component_attribute_value(registry, mComponentId, mAttributeName);
  sys::set_component_attribute_value(registry,
                                     mComponentId,
                                     mAttributeName,
                                     mUpdatedValue);
}

auto UpdateComponentAttributeCmd::MergeWith(const ACommand& cmd) -> bool
{
  if (GetId() == cmd.GetId()) {
    const auto& other = dynamic_cast<const UpdateComponentAttributeCmd&>(cmd);
    if (mComponentId == other.mComponentId && mAttributeName == other.mAttributeName) {
      mUpdatedValue = other.mUpdatedValue;
      return true;
    }
  }

  return false;
}

}  // namespace tactile