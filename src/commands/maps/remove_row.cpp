#include "remove_row.hpp"

#include "algorithm.hpp"
#include "map.hpp"
#include "tactile_qstring.hpp"

namespace tactile::cmd {

RemoveRow::RemoveRow(not_null<core::MapDocument*> document)
    : RepeatedMapCommand{document, TACTILE_QSTRING(u"Remove Row")}
{}

void RemoveRow::undo()
{
  QUndoCommand::undo();

  InvokeN(Times(), [this] { GetMap().AddRow(empty); });

  RestoreTiles();
  Redraw();
}

void RemoveRow::redo()
{
  QUndoCommand::redo();

  auto& map = GetMap();

  const auto endCol = map.ColumnCount();
  const auto endRow = map.RowCount();
  const auto beginRow = endRow - 1_row - row_t{Times()};

  ClearCache();
  SaveTiles({beginRow, endRow}, {0_col, endCol});

  InvokeN(Times(), [this] { GetMap().RemoveRow(); });

  Redraw();
}

}  // namespace tactile::cmd
