#include "remove_row.hpp"

#include "algorithm.hpp"
#include "map.hpp"
#include "tactile_qstring.hpp"

namespace tactile::cmd {

remove_row::remove_row(not_null<core::map_document*> document)
    : repeated_map_command{document, TACTILE_QSTRING(u"Remove Row")}
{}

void remove_row::undo()
{
  QUndoCommand::undo();

  invoke_n(times(), [this] { GetMap().add_row(empty); });

  RestoreTiles();
  Redraw();
}

void remove_row::redo()
{
  QUndoCommand::redo();

  auto& map = GetMap();

  const auto endCol = map.col_count();
  const auto endRow = map.row_count();
  const auto beginRow = endRow - 1_row - row_t{times()};

  ClearCache();
  SaveTiles({beginRow, endRow}, {0_col, endCol});

  invoke_n(times(), [this] { GetMap().remove_row(); });

  Redraw();
}

}  // namespace tactile::cmd
