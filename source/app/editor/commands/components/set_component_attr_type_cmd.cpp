#include "set_component_attr_type_cmd.hpp"

#include <utility>  // move

#include "core/systems/component_system.hpp"

namespace tactile {

set_component_attr_type_cmd::set_component_attr_type_cmd(registry_ref registry,
                                                         const component_id id,
                                                         std::string attribute,
                                                         const attribute_type type)
    : command_base{"Set Component Attribute Type"}
    , mRegistry{registry}
    , mComponentId{id}
    , mAttributeName{std::move(attribute)}
    , mNewType{type}
{}

void set_component_attr_type_cmd::undo()
{
  auto& registry = mRegistry.get();

  const auto previous = mPreviousValue.value();

  sys::set_component_attribute_type(registry,
                                    mComponentId,
                                    mAttributeName,
                                    previous.type());
  sys::set_component_attribute_value(registry, mComponentId, mAttributeName, previous);
}

void set_component_attr_type_cmd::redo()
{
  auto& registry = mRegistry.get();

  mPreviousValue =
      sys::get_component_attribute_value(registry, mComponentId, mAttributeName);
  sys::set_component_attribute_type(registry, mComponentId, mAttributeName, mNewType);
}

}  // namespace tactile