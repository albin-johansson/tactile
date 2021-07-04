#include "property_command.hpp"

#include <cassert>  // assert
#include <utility>  // move

#include "core/properties/property_context.hpp"
#include "core/tactile_error.hpp"

namespace Tactile {

APropertyCommand::APropertyCommand(NotNull<IPropertyContext*> context,
                                   std::string name)
    : ACommand{std::move(name)}
    , mContext{context}
{
  if (!mContext)
  {
    throw TactileError{"Cannot create property command from null context!"};
  }
}

auto APropertyCommand::GetContext() -> IPropertyContext&
{
  assert(mContext);
  return *mContext;
}

}  // namespace Tactile
