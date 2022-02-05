#include "create_component_attr_cmd.hpp"

#include <utility>  // move

#include "core/systems/component_system.hpp"

namespace tactile {

create_component_attr_cmd::create_component_attr_cmd(registry_ref registry,
                                                     const component_id id,
                                                     std::string name)
    : command_base{"Create Component Attribute"}
    , mRegistry{registry}
    , mComponentId{id}
    , mName{std::move(name)}
{}

void create_component_attr_cmd::undo()
{
  auto& registry = mRegistry.get();
  sys::remove_component_attribute(registry, mComponentId, mName);
}

void create_component_attr_cmd::redo()
{
  auto& registry = mRegistry.get();
  sys::make_component_attribute(registry, mComponentId, mName);
}

}  // namespace tactile
