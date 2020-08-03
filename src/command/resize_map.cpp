#include "resize_map.hpp"

#include "tactile_error.hpp"

namespace tactile::cmd {

resize_map::resize_map(model::core* core, int rows, int cols)
    : QUndoCommand{"Resize Map"}, m_core{core}, m_rows{rows}, m_cols{cols}
{
  if (!core) {
    throw tactile_error{"Cannot create resize_map command from null core!"};
  }
}

void resize_map::undo()
{
  QUndoCommand::undo();

  // This just reverts the size, but the state of the removed tiles is lost
  m_core->set_rows(m_oldRows);
  m_core->set_cols(m_oldCols);

  // TODO restore actual state of old tiles

  emit m_core->request_update();
}

void resize_map::redo()
{
  QUndoCommand::redo();

  m_oldRows = m_core->rows().value_or(1);
  m_oldCols = m_core->cols().value_or(1);

  m_core->set_rows(m_rows);
  m_core->set_cols(m_cols);

  emit m_core->request_update();
}

}  // namespace tactile::cmd
