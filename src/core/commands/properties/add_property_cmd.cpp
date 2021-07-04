#include "add_property_cmd.hpp"

#include <utility>  // move

#include "core/properties/property_context.hpp"

namespace Tactile {

AddPropertyCmd::AddPropertyCmd(NotNull<IPropertyContext*> context,
                               std::string name,
                               const PropertyType type)
    : APropertyCommand{context, "Add Property"}
    , mName{std::move(name)}
    , mType{type}
{}

void AddPropertyCmd::Undo()
{
  GetContext().RemoveProperty(mName);
}

void AddPropertyCmd::Redo()
{
  GetContext().AddProperty(mName, mType);
}

}  // namespace Tactile
