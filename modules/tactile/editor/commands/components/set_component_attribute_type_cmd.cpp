#include "set_component_attribute_type_cmd.hpp"

#include <utility>  // move

#include "core/systems/component_system.hpp"

namespace Tactile {

SetComponentAttributeTypeCmd::SetComponentAttributeTypeCmd(RegistryRef registry,
                                                           const ComponentID id,
                                                           std::string attribute,
                                                           const PropertyType type)
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

  Sys::SetComponentAttributeType(registry,
                                 mComponentId,
                                 mAttributeName,
                                 previous.GetType().value());
  Sys::SetComponentAttributeValue(registry, mComponentId, mAttributeName, previous);
}

void SetComponentAttributeTypeCmd::Redo()
{
  auto& registry = mRegistry.get();

  mPreviousValue =
      Sys::GetComponentAttributeValue(registry, mComponentId, mAttributeName);
  Sys::SetComponentAttributeType(registry, mComponentId, mAttributeName, mNewType);
}

}  // namespace Tactile