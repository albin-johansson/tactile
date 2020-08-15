#include "add_col.hpp"

#include "tactile_error.hpp"

using tactile::model::core;

namespace tactile::cmd {

add_col::add_col(core* core) : QUndoCommand{"Add Column"}, m_core{core}
{
  if (!core) {
    throw tactile_error{"Cannot create add_col command from null core!"};
  }
}

void add_col::undo()
{
  QUndoCommand::undo();

  m_core->remove_col();
}

void add_col::redo()
{
  QUndoCommand::redo();

  m_core->add_col();
}

}  // namespace tactile::cmd
