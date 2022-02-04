#include "create_component_attribute_cmd.hpp"

#include <utility>  // move

#include "core/systems/component_system.hpp"

namespace tactile {

CreateComponentAttributeCmd::CreateComponentAttributeCmd(registry_ref registry,
                                                         const component_id id,
                                                         std::string name)
    : command_base{"Create Component Attribute"}
    , mRegistry{registry}
    , mComponentId{id}
    , mName{std::move(name)}
{}

void CreateComponentAttributeCmd::undo()
{
  auto& registry = mRegistry.get();
  sys::remove_component_attribute(registry, mComponentId, mName);
}

void CreateComponentAttributeCmd::redo()
{
  auto& registry = mRegistry.get();
  sys::make_component_attribute(registry, mComponentId, mName);
}

}  // namespace tactile
