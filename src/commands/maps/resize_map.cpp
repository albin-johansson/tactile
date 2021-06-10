#include "resize_map.hpp"

#include "map.hpp"
#include "tactile_qstring.hpp"

using namespace tactile::core;

namespace tactile::cmd {

resize_map::resize_map(not_null<core::map_document*> document,
                       const row_t rows,
                       const col_t cols)
    : MapCommand{document, TACTILE_QSTRING(u"Resize Map")}
    , m_rows{rows}
    , m_cols{cols}
{}

void resize_map::undo()
{
  QUndoCommand::undo();

  auto& map = GetMap();

  map.set_row_count(m_oldRows);
  map.set_col_count(m_oldCols);

  if (lossy_resize())
  {
    RestoreTiles();
  }

  Redraw();
}

void resize_map::redo()
{
  QUndoCommand::redo();

  auto& map = GetMap();

  m_oldRows = map.row_count();
  m_oldCols = map.col_count();

  if (lossy_resize())
  {
    const auto rows = map.row_count();
    const auto cols = map.col_count();

    ClearCache();
    SaveTiles({rows - m_oldRows - m_rows, rows}, {0_col, cols});
    SaveTiles({0_row, rows}, {cols - m_oldCols - m_cols, cols});
  }

  map.set_row_count(m_rows);
  map.set_col_count(m_cols);

  Redraw();
}

}  // namespace tactile::cmd
