#include "add_col.hpp"

namespace tactile::cmd {

add_col::add_col(not_null<model::map*> map)
    : abstract_command{"Add Column", map}
{}

void add_col::undo()
{
  QUndoCommand::undo();

  m_map->remove_col();
}

void add_col::redo()
{
  QUndoCommand::redo();

  m_map->add_col();
}

}  // namespace tactile::cmd
