#include "remove_row.hpp"

#include "tactile_error.hpp"

using tactile::model::core;

namespace tactile::cmd {

remove_row::remove_row(model::core* core)
    : QUndoCommand{"Remove Row"}, m_core{core}
{
  if (!core) {
    throw tactile_error{"Cannot create remove_row command from null core!"};
  }
}

void remove_row::undo()
{
  QUndoCommand::undo();

  m_core->add_row();
}

void remove_row::redo()
{
  QUndoCommand::redo();

  m_core->remove_row();
}

}  // namespace tactile::cmd
