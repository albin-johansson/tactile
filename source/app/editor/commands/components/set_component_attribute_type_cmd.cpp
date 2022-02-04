#include "set_component_attribute_type_cmd.hpp"

#include <utility>  // move

#include "core/systems/component_system.hpp"

namespace tactile {

SetComponentAttributeTypeCmd::SetComponentAttributeTypeCmd(registry_ref registry,
                                                           const component_id id,
                                                           std::string attribute,
                                                           const attribute_type type)
    : command_base{"Set Component Attribute Type"}
    , mRegistry{registry}
    , mComponentId{id}
    , mAttributeName{std::move(attribute)}
    , mNewType{type}
{}

void SetComponentAttributeTypeCmd::undo()
{
  auto& registry = mRegistry.get();

  const auto previous = mPreviousValue.value();

  sys::set_component_attribute_type(registry,
                                    mComponentId,
                                    mAttributeName,
                                    previous.type());
  sys::set_component_attribute_value(registry, mComponentId, mAttributeName, previous);
}

void SetComponentAttributeTypeCmd::redo()
{
  auto& registry = mRegistry.get();

  mPreviousValue =
      sys::get_component_attribute_value(registry, mComponentId, mAttributeName);
  sys::set_component_attribute_type(registry, mComponentId, mAttributeName, mNewType);
}

}  // namespace tactile