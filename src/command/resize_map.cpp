#include "resize_map.hpp"

using namespace tactile::core;

namespace tactile::cmd {

resize_map::resize_map(core::map* map, row_t rows, col_t cols)
    : map_command{map, QStringLiteral(u"Resize Map")},
      m_rows{rows},
      m_cols{cols}
{}

void resize_map::undo()
{
  QUndoCommand::undo();

  auto* map = get_map();

  map->set_row_count(m_oldRows);
  map->set_col_count(m_oldCols);

  if (lossy_resize()) {
    restore_tiles();
  }
}

void resize_map::redo()
{
  QUndoCommand::redo();

  auto* map = get_map();

  m_oldRows = map->row_count();
  m_oldCols = map->col_count();

  if (lossy_resize()) {
    const auto rows = map->row_count();
    const auto cols = map->col_count();

    clear_cache();
    save_tiles({rows - m_oldRows - m_rows, rows}, {0_col, cols});
    save_tiles({0_row, rows}, {cols - m_oldCols - m_cols, cols});
  }

  map->set_row_count(m_rows);
  map->set_col_count(m_cols);
}

}  // namespace tactile::cmd
