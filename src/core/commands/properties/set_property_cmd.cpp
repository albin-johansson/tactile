#include "set_property_cmd.hpp"

#include <utility>  // move

#include "core/properties/property_context.hpp"

namespace Tactile {

SetPropertyCmd::SetPropertyCmd(NotNull<IPropertyContext*> context,
                               std::string name,
                               Property value)
    : APropertyCommand{context, "Update Property Value"}
    , mName{std::move(name)}
    , mValue{std::move(value)}
{}

void SetPropertyCmd::Undo()
{
  GetContext().SetProperty(mName, mPrevious.value());
  mPrevious.reset();
}

void SetPropertyCmd::Redo()
{
  auto& context = GetContext();
  mPrevious = context.GetProperty(mName);
  context.SetProperty(mName, mValue);
}

auto SetPropertyCmd::MergeWith(const ACommand& cmd) -> bool
{
  if (GetId() == cmd.GetId())
  {
    const auto& other = dynamic_cast<const SetPropertyCmd&>(cmd);
    if (mName == other.mName)
    {
      mValue = other.mValue;
      return true;
    }
  }

  return false;
}

}  // namespace Tactile
