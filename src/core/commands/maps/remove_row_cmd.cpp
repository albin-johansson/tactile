#include "remove_row_cmd.hpp"

#include "core/map_document.hpp"

namespace Tactile {

RemoveRowCmd::RemoveRowCmd(NotNull<MapDocument*> document)
    : MergeableMapCommand{document, "Remove Row(s)"}
{}

void RemoveRowCmd::Undo()
{
  auto& map = GetMap();

  for (int i = 0; i < GetAmount(); ++i)
  {
    map.AddRow(empty_tile);
  }

  RestoreTiles();
}

void RemoveRowCmd::Redo()
{
  auto& map = GetMap();

  const MapPosition begin{map.GetRowCount() - row_t{GetAmount()} - 1_row,
                          0_col};
  const MapPosition end{map.GetRowCount(), map.GetColumnCount()};

  ClearCache();
  SaveTiles(begin, end);

  for (int i = 0; i < GetAmount(); ++i)
  {
    map.RemoveRow();
  }
}

}  // namespace Tactile
