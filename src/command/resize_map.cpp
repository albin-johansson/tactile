#include "resize_map.hpp"

namespace tactile::cmd {

resize_map::resize_map(not_null<model::tilemap*> map, int rows, int cols)
    : abstract_command{"Resize Map", map}, m_rows{rows}, m_cols{cols}
{}

void resize_map::undo()
{
  QUndoCommand::undo();

  // This just reverts the size, but the state of the removed tiles is lost
  m_map->set_rows(m_oldRows);
  m_map->set_cols(m_oldCols);

  // TODO restore actual state of old tiles
}

void resize_map::redo()
{
  QUndoCommand::redo();

  m_oldRows = m_map->rows();
  m_oldCols = m_map->cols();

  m_map->set_rows(m_rows);
  m_map->set_cols(m_cols);
}

}  // namespace tactile::cmd
