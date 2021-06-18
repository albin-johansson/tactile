#include "resize_map.hpp"

#include "map.hpp"

namespace tactile::cmd {

ResizeMap::ResizeMap(NotNull<core::MapDocument*> document,
                     const row_t rows,
                     const col_t cols)
    : MapCommand{document, QStringLiteral(u"Resize Map")}
    , mRows{rows}
    , mCols{cols}
{}

void ResizeMap::undo()
{
  QUndoCommand::undo();

  auto& map = GetMap();

  map.SetRowCount(mOldRows);
  map.SetColumnCount(mOldCols);

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

  mOldRows = map.RowCount();
  mOldCols = map.ColumnCount();

  if (IsLossyResize())
  {
    const auto rows = map.RowCount();
    const auto cols = map.ColumnCount();

    ClearCache();
    SaveTiles({rows - mOldRows - mRows, rows}, {0_col, cols});
    SaveTiles({0_row, rows}, {cols - mOldCols - mCols, cols});
  }

  map.SetRowCount(mRows);
  map.SetColumnCount(mCols);

  Redraw();
}

}  // namespace tactile::cmd
