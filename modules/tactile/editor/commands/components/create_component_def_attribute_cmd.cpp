#include "create_component_def_attribute_cmd.hpp"

#include <utility>  // move

#include "core/systems/component_system.hpp"

namespace Tactile {

CreateComponentDefAttributeCmd::CreateComponentDefAttributeCmd(RegistryRef registry,
                                                               const ComponentID id,
                                                               std::string name)
    : ACommand{"Create Component Attribute"}
    , mRegistry{registry}
    , mComponentId{id}
    , mName{std::move(name)}
{}

void CreateComponentDefAttributeCmd::Undo()
{
  auto& registry = mRegistry.get();
  Sys::RemoveComponentAttribute(registry, mComponentId, mName);
}

void CreateComponentDefAttributeCmd::Redo()
{
  auto& registry = mRegistry.get();
  Sys::CreateComponentAttribute(registry, mComponentId, mName);
}

}  // namespace Tactile
