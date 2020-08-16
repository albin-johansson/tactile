#include "add_row.hpp"

namespace tactile::cmd {

add_row::add_row(not_null<model::tilemap*> map)
    : abstract_command{"Add Row", map}
{}

void add_row::undo()
{
  QUndoCommand::undo();

  m_map->remove_row();
}

void add_row::redo()
{
  QUndoCommand::redo();

  m_map->add_row();
}

}  // namespace tactile::cmd
