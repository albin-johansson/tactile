#include "create_component_attribute_cmd.hpp"

#include <utility>  // move

#include "core/systems/component_system.hpp"

namespace tactile {

CreateComponentAttributeCmd::CreateComponentAttributeCmd(RegistryRef registry,
                                                         const component_id id,
                                                         std::string name)
    : ACommand{"Create Component Attribute"}
    , mRegistry{registry}
    , mComponentId{id}
    , mName{std::move(name)}
{}

void CreateComponentAttributeCmd::Undo()
{
  auto& registry = mRegistry.get();
  sys::remove_component_attribute(registry, mComponentId, mName);
}

void CreateComponentAttributeCmd::Redo()
{
  auto& registry = mRegistry.get();
  sys::make_component_attribute(registry, mComponentId, mName);
}

}  // namespace tactile
