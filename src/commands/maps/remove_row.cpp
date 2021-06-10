#include "remove_row.hpp"

#include "algorithm.hpp"
#include "map.hpp"
#include "tactile_qstring.hpp"

namespace tactile::cmd {

RemoveRow::RemoveRow(not_null<core::map_document*> document)
    : RepeatedMapCommand{document, TACTILE_QSTRING(u"Remove Row")}
{}

void RemoveRow::undo()
{
  QUndoCommand::undo();

  invoke_n(Times(), [this] { GetMap().add_row(empty); });

  RestoreTiles();
  Redraw();
}

void RemoveRow::redo()
{
  QUndoCommand::redo();

  auto& map = GetMap();

  const auto endCol = map.col_count();
  const auto endRow = map.row_count();
  const auto beginRow = endRow - 1_row - row_t{Times()};

  ClearCache();
  SaveTiles({beginRow, endRow}, {0_col, endCol});

  invoke_n(Times(), [this] { GetMap().remove_row(); });

  Redraw();
}

}  // namespace tactile::cmd
