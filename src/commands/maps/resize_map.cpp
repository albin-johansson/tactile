#include "resize_map.hpp"

#include "map.hpp"
#include "tactile_qstring.hpp"

using namespace tactile::core;

namespace tactile::cmd {

ResizeMap::ResizeMap(not_null<core::map_document*> document,
                     const row_t rows,
                     const col_t cols)
    : MapCommand{document, TACTILE_QSTRING(u"Resize Map")}
    , mRows{rows}
    , mCols{cols}
{}

void ResizeMap::undo()
{
  QUndoCommand::undo();

  auto& map = GetMap();

  map.set_row_count(mOldRows);
  map.set_col_count(mOldCols);

  if (IsLossyResize())
  {
    RestoreTiles();
  }

  Redraw();
}

void ResizeMap::redo()
{
  QUndoCommand::redo();

  auto& map = GetMap();

  mOldRows = map.row_count();
  mOldCols = map.col_count();

  if (IsLossyResize())
  {
    const auto rows = map.row_count();
    const auto cols = map.col_count();

    ClearCache();
    SaveTiles({rows - mOldRows - mRows, rows}, {0_col, cols});
    SaveTiles({0_row, rows}, {cols - mOldCols - mCols, cols});
  }

  map.set_row_count(mRows);
  map.set_col_count(mCols);

  Redraw();
}

}  // namespace tactile::cmd
