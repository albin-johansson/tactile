#include "remove_component_attribute_cmd.hpp"

#include <utility>  // move

#include "core/systems/component_system.hpp"

namespace tactile {

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
  sys::make_component_attribute(registry,
                                mComponentId,
                                mAttributeName,
                                mPreviousDefault.value());
  mPreviousDefault.reset();
}

void RemoveComponentAttributeCmd::Redo()
{
  auto& registry = mRegistry.get();
  mPreviousDefault =
      sys::get_component_attribute_value(registry, mComponentId, mAttributeName);
  sys::remove_component_attribute(registry, mComponentId, mAttributeName);
}

}  // namespace tactile