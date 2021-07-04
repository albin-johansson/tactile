#include "remove_property_cmd.hpp"

#include <utility>  // move

#include "core/properties/property_context.hpp"

namespace Tactile {

RemovePropertyCmd::RemovePropertyCmd(NotNull<IPropertyContext*> context,
                                     std::string name)
    : APropertyCommand{context, "Remove Property"}
    , mName{std::move(name)}
{}

void RemovePropertyCmd::Undo()
{
  GetContext().AddProperty(mName, mProperty.value());
  mProperty.reset();
}

void RemovePropertyCmd::Redo()
{
  auto& context = GetContext();
  mProperty = context.GetProperty(mName);
  context.RemoveProperty(mName);
}

}  // namespace Tactile
