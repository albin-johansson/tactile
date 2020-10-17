#include "add_col.hpp"

#include "algorithm.hpp"

namespace tactile::cmd {

add_col::add_col(core::map* map)
    : row_col_command{QStringLiteral(u"Add Column"), map}
{}

void add_col::undo()
{
  QUndoCommand::undo();

  invoke_n(amount(), [this] { m_map->remove_col(); });
}

void add_col::redo()
{
  QUndoCommand::redo();

  invoke_n(amount(), [this] { m_map->add_col(empty); });
}

}  // namespace tactile::cmd
