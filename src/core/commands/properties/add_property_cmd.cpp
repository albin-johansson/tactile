#include "add_property_cmd.hpp"

#include <utility>  // move

#include "core/properties/property_context.hpp"
#include "core/tactile_error.hpp"

namespace Tactile {

AddPropertyCmd::AddPropertyCmd(NotNull<IPropertyContext*> context,
                               std::string name,
                               const PropertyType type)
    : ACommand{"Add Property"}
    , mContext{context}
    , mName{std::move(name)}
    , mType{type}
{
  if (!mContext)
  {
    throw TactileError{"Cannot create command from null property context!"};
  }
}

void AddPropertyCmd::Undo()
{
  mContext->RemoveProperty(mName);
}

void AddPropertyCmd::Redo()
{
  mContext->AddProperty(mName, mType);
}

}  // namespace Tactile
