#include "create_component_attribute_cmd.hpp"

#include <utility>  // move

#include "core/systems/component_system.hpp"

namespace tactile {

CreateComponentAttributeCmd::CreateComponentAttributeCmd(RegistryRef registry,
                                                         const ComponentID id,
                                                         std::string name)
    : ACommand{"Create Component Attribute"}
    , mRegistry{registry}
    , mComponentId{id}
    , mName{std::move(name)}
{}

void CreateComponentAttributeCmd::Undo()
{
  auto& registry = mRegistry.get();
  sys::RemoveComponentAttribute(registry, mComponentId, mName);
}

void CreateComponentAttributeCmd::Redo()
{
  auto& registry = mRegistry.get();
  sys::CreateComponentAttribute(registry, mComponentId, mName);
}

}  // namespace tactile
