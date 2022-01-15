#include "change_property_type_cmd.hpp"

#include <utility>  // move

#include "core/systems/property_system.hpp"

namespace Tactile {

ChangePropertyTypeCmd::ChangePropertyTypeCmd(RegistryRef registry,
                                             std::string name,
                                             PropertyType type)
    : ACommand{"Change Property Type"}
    , mRegistry{registry}
    , mContextId{Sys::GetCurrentContextId(mRegistry)}
    , mName{std::move(name)}
    , mPropertyType{type}
{}

void ChangePropertyTypeCmd::Undo()
{
  const auto& value = mPreviousValue.value();
  const auto type = value.GetType().value();

  auto& context = Sys::GetContext(mRegistry, mContextId);
  Sys::ChangePropertyType(mRegistry, context, mName, type);
  Sys::UpdateProperty(mRegistry, context, mName, value);

  mPreviousValue.reset();
}

void ChangePropertyTypeCmd::Redo()
{
  auto& context = Sys::GetContext(mRegistry, mContextId);
  mPreviousValue = Sys::GetProperty(mRegistry, context, mName).value;
  Sys::ChangePropertyType(mRegistry, context, mName, mPropertyType);
}

}  // namespace Tactile
