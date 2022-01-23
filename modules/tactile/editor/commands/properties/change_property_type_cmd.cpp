#include "change_property_type_cmd.hpp"

#include <utility>  // move

#include "core/systems/property_system.hpp"

namespace tactile {

ChangePropertyTypeCmd::ChangePropertyTypeCmd(RegistryRef registry,
                                             std::string name,
                                             PropertyType type)
    : ACommand{"Change Property Type"}
    , mRegistry{registry}
    , mContextId{sys::GetCurrentContextId(mRegistry)}
    , mName{std::move(name)}
    , mPropertyType{type}
{}

void ChangePropertyTypeCmd::Undo()
{
  const auto& value = mPreviousValue.value();
  const auto type = value.type();

  auto& context = sys::GetContext(mRegistry, mContextId);
  sys::ChangePropertyType(mRegistry, context, mName, type);
  sys::UpdateProperty(mRegistry, context, mName, value);

  mPreviousValue.reset();
}

void ChangePropertyTypeCmd::Redo()
{
  auto& context = sys::GetContext(mRegistry, mContextId);
  mPreviousValue = sys::GetProperty(mRegistry, context, mName).value;
  sys::ChangePropertyType(mRegistry, context, mName, mPropertyType);
}

}  // namespace tactile
