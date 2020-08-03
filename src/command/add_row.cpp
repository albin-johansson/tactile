#include "add_row.hpp"

#include "tactile_error.hpp"

using tactile::model::core;

namespace tactile::cmd {

add_row::add_row(core* core) : QUndoCommand{"Add Row"}, m_core{core}
{
  if (!core) {
    throw tactile_error{"Cannot create add_row command from null core!"};
  }
}

void add_row::undo()
{
  QUndoCommand::undo();

  m_core->handle_remove_row();
}

void add_row::redo()
{
  QUndoCommand::redo();

  m_core->handle_add_row();
}

}  // namespace tactile::cmd
