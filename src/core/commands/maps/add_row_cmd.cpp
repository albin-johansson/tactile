#include "add_row_cmd.hpp"

#include "core/map/map.hpp"

namespace Tactile {

AddRowCmd::AddRowCmd(NotNull<MapDocument*> document)
    : MapCommand{document, "Add Row"}
{}

void AddRowCmd::Undo()
{
  auto& map = GetMap();
  map.RemoveRow();
}

void AddRowCmd::Redo()
{
  auto& map = GetMap();
  map.AddRow(empty_tile);
}

}  // namespace Tactile
