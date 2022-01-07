#include "duplicate_component_attribute_cmd.hpp"

#include <utility>  // move

#include "core/systems/component_system.hpp"

namespace Tactile {

DuplicateComponentAttributeCmd::DuplicateComponentAttributeCmd(RegistryRef registry,
                                                               ComponentID componentId,
                                                               std::string attribute)
    : ACommand{"Duplicate Component Attribute"}
    , mRegistry{registry}
    , mComponentId{componentId}
    , mAttributeName{std::move(attribute)}
{}

void DuplicateComponentAttributeCmd::Undo()
{
  auto& registry = mRegistry.get();
  Sys::RemoveComponentAttribute(registry, mComponentId, mDuplicatedName.value());
  mDuplicatedName.reset();
}

void DuplicateComponentAttributeCmd::Redo()
{
  auto& registry = mRegistry.get();
  mDuplicatedName =
      Sys::DuplicateComponentAttribute(registry, mComponentId, mAttributeName);
}

}  // namespace Tactile