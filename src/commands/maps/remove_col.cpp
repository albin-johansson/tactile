#include "remove_col.hpp"

#include "algorithm.hpp"
#include "map.hpp"
#include "tactile_qstring.hpp"

namespace tactile::cmd {

remove_col::remove_col(not_null<core::map_document*> document)
    : repeated_map_command{document, TACTILE_QSTRING(u"Remove Column")}
{}

void remove_col::undo()
{
  QUndoCommand::undo();

  invoke_n(times(), [&] { GetMap().add_col(empty); });

  RestoreTiles();
  Redraw();
}

void remove_col::redo()
{
  QUndoCommand::redo();

  auto& map = GetMap();

  const auto endRow = map.row_count();
  const auto endCol = map.col_count();
  const auto beginCol = endCol - 1_col - col_t{times()};

  ClearCache();
  SaveTiles({0_row, endRow}, {beginCol, endCol});

  invoke_n(times(), [&] { GetMap().remove_col(); });

  Redraw();
}

}  // namespace tactile::cmd
