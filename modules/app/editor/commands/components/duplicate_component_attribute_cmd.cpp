#include "duplicate_component_attribute_cmd.hpp"

#include <utility>  // move

#include "core/systems/component_system.hpp"

namespace tactile {

DuplicateComponentAttributeCmd::DuplicateComponentAttributeCmd(registry_ref registry,
                                                               component_id componentId,
                                                               std::string attribute)
    : command_base{"Duplicate Component Attribute"}
    , mRegistry{registry}
    , mComponentId{componentId}
    , mAttributeName{std::move(attribute)}
{}

void DuplicateComponentAttributeCmd::undo()
{
  auto& registry = mRegistry.get();
  sys::remove_component_attribute(registry, mComponentId, mDuplicatedName.value());
  mDuplicatedName.reset();
}

void DuplicateComponentAttributeCmd::redo()
{
  auto& registry = mRegistry.get();
  mDuplicatedName =
      sys::duplicate_component_attribute(registry, mComponentId, mAttributeName);
}

}  // namespace tactile