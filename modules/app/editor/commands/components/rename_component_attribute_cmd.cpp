#include "rename_component_attribute_cmd.hpp"

#include <utility>  // move

#include "core/systems/component_system.hpp"

namespace tactile {

RenameComponentAttributeCmd::RenameComponentAttributeCmd(registry_ref registry,
                                                         const component_id id,
                                                         std::string previousName,
                                                         std::string updatedName)
    : command_base{"Rename Component Attribute"}
    , mRegistry{registry}
    , mComponentId{id}
    , mPreviousName{std::move(previousName)}
    , mUpdatedName{std::move(updatedName)}
{}

void RenameComponentAttributeCmd::undo()
{
  auto& registry = mRegistry.get();
  sys::rename_component_attribute(registry, mComponentId, mUpdatedName, mPreviousName);
}

void RenameComponentAttributeCmd::redo()
{
  auto& registry = mRegistry.get();
  sys::rename_component_attribute(registry, mComponentId, mPreviousName, mUpdatedName);
}

}  // namespace tactile