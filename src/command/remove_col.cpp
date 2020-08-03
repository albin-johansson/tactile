#include "remove_col.hpp"

#include "tactile_error.hpp"

using tactile::model::core;

namespace tactile::cmd {

remove_col::remove_col(model::core* core)
    : QUndoCommand{"Remove Column"}, m_core{core}
{
  if (!core) {
    throw tactile_error{"Cannot create remove_col command from null core!"};
  }
}

void remove_col::undo()
{
  QUndoCommand::undo();

  m_core->handle_add_col();
}

void remove_col::redo()
{
  QUndoCommand::redo();

  m_core->handle_remove_col();
}

}  // namespace tactile::cmd
