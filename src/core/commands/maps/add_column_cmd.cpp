#include "add_column_cmd.hpp"

#include "core/map_document.hpp"

namespace Tactile {

AddColumnCmd::AddColumnCmd(NotNull<MapDocument*> document)
    : MapCommand{document, "Add Column"}
{}

void AddColumnCmd::Undo()
{
  auto& map = GetMap();
  map.RemoveColumn();
}

void AddColumnCmd::Redo()
{
  auto& map = GetMap();
  map.AddColumn(empty_tile);
}

}  // namespace Tactile
