#include "remove_row.hpp"

namespace tactile::cmd {

remove_row::remove_row(not_null<core::map*> map)
    : abstract_command{"Remove Row", map}
{}

void remove_row::undo()
{
  QUndoCommand::undo();

  m_map->add_row();
}

void remove_row::redo()
{
  QUndoCommand::redo();

  m_map->remove_row();
}

}  // namespace tactile::cmd
