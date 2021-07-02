#include "remove_row_cmd.hpp"

#include "core/map_document.hpp"

namespace Tactile {

RemoveRowCmd::RemoveRowCmd(NotNull<MapDocument*> document)
    : MapCommand{document, "Remove Row"}
{}

void RemoveRowCmd::Undo()
{
  auto& map = GetMap();
  map.AddRow(empty_tile);

  RestoreTiles();
}

void RemoveRowCmd::Redo()
{
  auto& map = GetMap();

  const auto begin = MapPosition{map.GetRowCount() - 1_row, 0_col};
  const auto end = MapPosition{map.GetRowCount(), map.GetColumnCount()};

  ClearCache();
  SaveTiles(begin, end);

  map.RemoveRow();
}

}  // namespace Tactile
