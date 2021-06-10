#include "remove_column.hpp"

#include "algorithm.hpp"
#include "map.hpp"
#include "tactile_qstring.hpp"

namespace tactile::cmd {

RemoveColumn::RemoveColumn(not_null<core::MapDocument*> document)
    : RepeatedMapCommand{document, TACTILE_QSTRING(u"Remove Column")}
{}

void RemoveColumn::undo()
{
  QUndoCommand::undo();

  InvokeN(Times(), [&] { GetMap().AddColumn(empty); });

  RestoreTiles();
  Redraw();
}

void RemoveColumn::redo()
{
  QUndoCommand::redo();

  auto& map = GetMap();

  const auto endRow = map.RowCount();
  const auto endCol = map.ColumnCount();
  const auto beginCol = endCol - 1_col - col_t{Times()};

  ClearCache();
  SaveTiles({0_row, endRow}, {beginCol, endCol});

  InvokeN(Times(), [&] { GetMap().RemoveColumn(); });

  Redraw();
}

}  // namespace tactile::cmd
