#include "remove_row.hpp"

#include "algorithm.hpp"

namespace tactile::cmd {

remove_row::remove_row(core::map* map)
    : row_col_command{QStringLiteral(u"Remove Row"), map}
{}

void remove_row::undo()
{
  QUndoCommand::undo();

  invoke_n(amount(), [this] { m_map->add_row(); });
}

void remove_row::redo()
{
  QUndoCommand::redo();

  invoke_n(amount(), [this] { m_map->remove_row(); });
}

}  // namespace tactile::cmd
