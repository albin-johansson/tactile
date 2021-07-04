#include "change_property_type_cmd.hpp"

#include <utility>  // move

#include "core/properties/property_context.hpp"

namespace Tactile {

ChangePropertyTypeCmd::ChangePropertyTypeCmd(NotNull<IPropertyContext*> context,
                                             std::string name,
                                             const PropertyType type)
    : APropertyCommand{context, "Change Property Type"}
    , mName{std::move(name)}
    , mType{type}
{}

void ChangePropertyTypeCmd::Undo()
{
  auto& context = GetContext();

  const auto previous = mPreviousValue.value();
  context.ChangePropertyType(mName, previous.GetType().value());
  context.SetProperty(mName, previous);

  mPreviousValue.reset();
}

void ChangePropertyTypeCmd::Redo()
{
  auto& context = GetContext();
  mPreviousValue = context.GetProperty(mName);
  context.ChangePropertyType(mName, mType);
}

}  // namespace Tactile
