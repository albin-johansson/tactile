#include "add_property_cmd.hpp"

#include <utility>  // move

#include "core/systems/property_system.hpp"

namespace tactile {

AddPropertyCmd::AddPropertyCmd(RegistryRef registry,
                               std::string name,
                               const attribute_type type)
    : ACommand{"Add Property"}
    , mRegistry{registry}
    , mContextId{sys::GetCurrentContextId(mRegistry)}
    , mName{std::move(name)}
    , mType{type}
{}

void AddPropertyCmd::Undo()
{
  auto& context = sys::GetContext(mRegistry, mContextId);
  sys::RemoveProperty(mRegistry, context, mName);
}

void AddPropertyCmd::Redo()
{
  auto& context = sys::GetContext(mRegistry, mContextId);
  sys::AddProperty(mRegistry, context, mName, mType);
}

}  // namespace tactile
