#include "duplicate_component_attr_cmd.hpp"

#include <utility>  // move

#include "core/systems/component_system.hpp"

namespace tactile {

duplicate_component_attr_cmd::duplicate_component_attr_cmd(registry_ref registry,
                                                           const component_id componentId,
                                                           std::string attribute)
    : command_base{"Duplicate Component Attribute"}
    , mRegistry{registry}
    , mComponentId{componentId}
    , mAttributeName{std::move(attribute)}
{}

void duplicate_component_attr_cmd::undo()
{
  auto& registry = mRegistry.get();
  sys::remove_component_attribute(registry, mComponentId, mDuplicatedName.value());
  mDuplicatedName.reset();
}

void duplicate_component_attr_cmd::redo()
{
  auto& registry = mRegistry.get();
  mDuplicatedName =
      sys::duplicate_component_attribute(registry, mComponentId, mAttributeName);
}

}  // namespace tactile