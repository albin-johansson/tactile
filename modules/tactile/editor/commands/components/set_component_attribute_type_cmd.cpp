#include "set_component_attribute_type_cmd.hpp"

#include <utility>  // move

#include "core/systems/component_system.hpp"

namespace tactile {

SetComponentAttributeTypeCmd::SetComponentAttributeTypeCmd(RegistryRef registry,
                                                           const ComponentID id,
                                                           std::string attribute,
                                                           const attribute_type type)
    : ACommand{"Set Component Attribute Type"}
    , mRegistry{registry}
    , mComponentId{id}
    , mAttributeName{std::move(attribute)}
    , mNewType{type}
{}

void SetComponentAttributeTypeCmd::Undo()
{
  auto& registry = mRegistry.get();

  const auto previous = mPreviousValue.value();

  sys::set_component_attribute_type(registry,
                                    mComponentId,
                                    mAttributeName,
                                    previous.type());
  sys::set_component_attribute_value(registry, mComponentId, mAttributeName, previous);
}

void SetComponentAttributeTypeCmd::Redo()
{
  auto& registry = mRegistry.get();

  mPreviousValue =
      sys::get_component_attribute_value(registry, mComponentId, mAttributeName);
  sys::set_component_attribute_type(registry, mComponentId, mAttributeName, mNewType);
}

}  // namespace tactile