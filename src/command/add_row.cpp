#include "add_row.hpp"

#include "algorithm.hpp"

namespace tactile::cmd {

add_row::add_row(core::map* map)
    : row_col_command{QStringLiteral(u"Add Row"), map}
{}

void add_row::undo()
{
  QUndoCommand::undo();

  do_n(amount(), [this] { m_map->remove_row(); });
}

void add_row::redo()
{
  QUndoCommand::redo();

  do_n(amount(), [this] { m_map->add_row(); });
}

}  // namespace tactile::cmd
