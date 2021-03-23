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

  invoke_n(times(), [this] {
    get_map().add_row(empty);
  });

  restore_tiles();
  redraw();
}

void remove_row::redo()
{
  QUndoCommand::redo();

  auto& map = get_map();

  const auto endCol = map.col_count();
  const auto endRow = map.row_count();
  const auto beginRow = endRow - 1_row - row_t{times()};

  clear_cache();
  save_tiles({beginRow, endRow}, {0_col, endCol});

  invoke_n(times(), [this] {
    get_map().remove_row();
  });

  redraw();
}

}  // namespace tactile::cmd
