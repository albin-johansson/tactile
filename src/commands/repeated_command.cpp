#include "repeated_command.hpp"

namespace tactile::cmd {

auto RepeatedCommand::mergeWith(const QUndoCommand* other) -> bool
{
  if (other->id() == id())
  {
    if (auto* ptr = dynamic_cast<const RepeatedCommand*>(other))
    {
      mAmount += ptr->mAmount;
      return true;
    }
  }
  return false;
}

}  // namespace tactile::cmd
