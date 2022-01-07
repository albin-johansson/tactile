#include "remove_component_attribute_cmd.hpp"

#include <utility>  // move

#include "core/systems/component_system.hpp"

namespace Tactile {

RemoveComponentAttributeCmd::RemoveComponentAttributeCmd(RegistryRef registry,
                                                         const ComponentID componentId,
                                                         std::string attribute)
    : ACommand{"Remove Component Attribute"}
    , mRegistry{registry}
    , mComponentId{componentId}
    , mAttributeName{std::move(attribute)}
{}

void RemoveComponentAttributeCmd::Undo()
{
  auto& registry = mRegistry.get();
  Sys::CreateComponentAttribute(registry,
                                mComponentId,
                                mAttributeName,
                                mPreviousDefault.value());
  mPreviousDefault.reset();
}

void RemoveComponentAttributeCmd::Redo()
{
  auto& registry = mRegistry.get();
  mPreviousDefault =
      Sys::GetComponentAttributeValue(registry, mComponentId, mAttributeName);
  Sys::RemoveComponentAttribute(registry, mComponentId, mAttributeName);
}

}  // namespace Tactile