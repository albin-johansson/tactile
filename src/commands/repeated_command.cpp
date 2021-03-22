#include "repeated_command.hpp"

namespace tactile::cmd {

auto repeated_command::mergeWith(const QUndoCommand* other) -> bool
{
  if (other->id() == id())
  {
    if (auto* ptr = dynamic_cast<const repeated_command*>(other))
    {
      m_amount += ptr->m_amount;
      return true;
    }
  }
  return false;
}

}  // namespace tactile::cmd
