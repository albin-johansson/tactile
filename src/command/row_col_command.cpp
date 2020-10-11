#include "row_col_command.hpp"

namespace tactile::cmd {

row_col_command::row_col_command(const QString& name, core::map* map)
    : abstract_command{name, map}
{}

auto row_col_command::mergeWith(const QUndoCommand* other) -> bool
{
  if (other->id() == id()) {
    if (auto* ptr = dynamic_cast<const row_col_command*>(other)) {
      m_amount += ptr->m_amount;
      return true;
    }
  }
  return false;
}

}  // namespace tactile::cmd
