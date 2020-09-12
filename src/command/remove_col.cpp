#include "remove_col.hpp"

#include "algorithm.hpp"

namespace tactile::cmd {

remove_col::remove_col(not_null<core::map*> map)
    : row_col_command{QStringLiteral(u"Remove Column"), map}
{}

void remove_col::undo()
{
  QUndoCommand::undo();

  do_n(amount(), [this] { m_map->add_col(); });
}

void remove_col::redo()
{
  QUndoCommand::redo();

  do_n(amount(), [this] { m_map->remove_col(); });
}

}  // namespace tactile::cmd
