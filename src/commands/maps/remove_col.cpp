#include "remove_col.hpp"

#include "algorithm.hpp"
#include "tactile_qstring.hpp"

namespace tactile::cmd {

remove_col::remove_col(core::map* map)
    : repeated_map_command{map, TACTILE_QSTRING(u"Remove Column")}
{}

void remove_col::undo()
{
  QUndoCommand::undo();
  invoke_n(times(), [&] {
    get_map()->add_col(empty);
  });
  restore_tiles();
}

void remove_col::redo()
{
  QUndoCommand::redo();

  auto* map = get_map();

  const auto endRow = map->row_count();
  const auto endCol = map->col_count();
  const auto beginCol = endCol - 1_col - col_t{times()};

  clear_cache();
  save_tiles({0_row, endRow}, {beginCol, endCol});
  invoke_n(times(), [&] {
    get_map()->remove_col();
  });
}

}  // namespace tactile::cmd
