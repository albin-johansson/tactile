#include "add_property_cmd.hpp"

#include <utility>  // move

#include "core/systems/property_system.hpp"

namespace tactile {

AddPropertyCmd::AddPropertyCmd(RegistryRef registry,
                               std::string name,
                               const PropertyType type)
    : ACommand{"Add Property"}
    , mRegistry{registry}
    , mContextId{Sys::GetCurrentContextId(mRegistry)}
    , mName{std::move(name)}
    , mType{type}
{}

void AddPropertyCmd::Undo()
{
  auto& context = Sys::GetContext(mRegistry, mContextId);
  Sys::RemoveProperty(mRegistry, context, mName);
}

void AddPropertyCmd::Redo()
{
  auto& context = Sys::GetContext(mRegistry, mContextId);
  Sys::AddProperty(mRegistry, context, mName, mType);
}

}  // namespace tactile
