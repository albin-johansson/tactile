#include "change_property_type_cmd.hpp"

#include <utility>  // move

#include "core/systems/property_system.hpp"

namespace Tactile {

ChangePropertyTypeCmd::ChangePropertyTypeCmd(Ref<entt::registry> registry,
                                             std::string name,
                                             PropertyType type)
    : ACommand{"Change Property Type"}
    , mRegistry{registry}
    , mContextId{Sys::GetCurrentContext(mRegistry).id}
    , mName{std::move(name)}
    , mPropertyType{type}
{}

void ChangePropertyTypeCmd::Undo()
{
  const auto& value = mPreviousValue.value();
  const auto type = value.GetType().value();

  Sys::ChangePropertyType(mRegistry, mContextId, mName, type);
  Sys::UpdateProperty(mRegistry, mContextId, mName, value);

  mPreviousValue.reset();
}

void ChangePropertyTypeCmd::Redo()
{
  mPreviousValue = Sys::GetPropertyValue(mRegistry, mContextId, mName);
  Sys::ChangePropertyType(mRegistry, mContextId, mName, mPropertyType);
}

}  // namespace Tactile
