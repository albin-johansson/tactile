#include "rename_component_attribute_cmd.hpp"

#include <utility>  // move

#include "core/systems/component_system.hpp"

namespace Tactile {

RenameComponentAttributeCmd::RenameComponentAttributeCmd(RegistryRef registry,
                                                         const ComponentID id,
                                                         std::string previousName,
                                                         std::string updatedName)
    : ACommand{"Rename Component Attribute"}
    , mRegistry{registry}
    , mComponentId{id}
    , mPreviousName{std::move(previousName)}
    , mUpdatedName{std::move(updatedName)}
{}

void RenameComponentAttributeCmd::Undo()
{
  auto& registry = mRegistry.get();
  Sys::RenameComponentAttribute(registry, mComponentId, mUpdatedName, mPreviousName);
}

void RenameComponentAttributeCmd::Redo()
{
  auto& registry = mRegistry.get();
  Sys::RenameComponentAttribute(registry, mComponentId, mPreviousName, mUpdatedName);
}

}  // namespace Tactile