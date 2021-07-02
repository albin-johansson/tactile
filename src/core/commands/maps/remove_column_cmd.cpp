#include "remove_column_cmd.hpp"

#include "core/map_document.hpp"

namespace Tactile {

RemoveColumnCmd::RemoveColumnCmd(NotNull<MapDocument*> document)
    : MapCommand{document, "Remove Column"}
{}

void RemoveColumnCmd::Undo()
{
  auto& map = GetMap();
  map.AddColumn(empty_tile);

  RestoreTiles();
}

void RemoveColumnCmd::Redo()
{
  auto& map = GetMap();

  const auto begin = MapPosition{0_row, map.GetColumnCount() - 1_col};
  const auto end = MapPosition{map.GetRowCount(), map.GetColumnCount()};

  ClearCache();
  SaveTiles(begin, end);

  map.RemoveColumn();
}

}  // namespace Tactile
