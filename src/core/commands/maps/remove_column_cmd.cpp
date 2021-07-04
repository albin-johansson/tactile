#include "remove_column_cmd.hpp"

#include "core/map_document.hpp"

namespace Tactile {

RemoveColumnCmd::RemoveColumnCmd(NotNull<MapDocument*> document)
    : MergeableMapCommand{document, "Remove Column(s)"}
{}

void RemoveColumnCmd::Undo()
{
  auto& map = GetMap();

  for (int i = 0; i < GetAmount(); ++i)
  {
    map.AddColumn(empty_tile);
  }

  RestoreTiles();
}

void RemoveColumnCmd::Redo()
{
  auto& map = GetMap();

  const MapPosition begin{0_row,
                          map.GetColumnCount() - AsColumn(GetAmount()) - 1_col};
  const MapPosition end{map.GetRowCount(), map.GetColumnCount()};

  ClearCache();
  SaveTiles(begin, end);

  for (int i = 0; i < GetAmount(); ++i)
  {
    map.RemoveColumn();
  }
}

}  // namespace Tactile
