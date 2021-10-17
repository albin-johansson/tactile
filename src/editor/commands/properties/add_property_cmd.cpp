#include "add_property_cmd.hpp"

#include <cassert>  // assert
#include <utility>  // move

#include "core/systems/property_system.hpp"

namespace Tactile {

AddPropertyCmd::AddPropertyCmd(Ref<entt::registry> registry,
                               std::string name,
                               const PropertyType type)
    : ACommand{"Add Property"}
    , mRegistry{registry}
    , mContextId{Sys::GetCurrentContext(mRegistry).id}
    , mName{std::move(name)}
    , mType{type}
{}

void AddPropertyCmd::Undo()
{
  Sys::RemoveProperty(mRegistry, mContextId, mName);
}

void AddPropertyCmd::Redo()
{
  Sys::AddProperty(mRegistry, mContextId, mName, mType);
}

}  // namespace Tactile
