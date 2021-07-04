#include "add_row_cmd.hpp"

#include "core/map/map.hpp"

namespace Tactile {

AddRowCmd::AddRowCmd(NotNull<MapDocument*> document)
    : MergeableMapCommand{document, "Add Row(s)"}
{}

void AddRowCmd::Undo()
{
  auto& map = GetMap();
  for (int i = 0; i < GetAmount(); ++i)
  {
    map.RemoveRow();
  }
}

void AddRowCmd::Redo()
{
  auto& map = GetMap();
  for (int i = 0; i < GetAmount(); ++i)
  {
    map.AddRow(empty_tile);
  }
}

}  // namespace Tactile
