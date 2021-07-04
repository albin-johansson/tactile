#include "add_column_cmd.hpp"

#include "core/map_document.hpp"

namespace Tactile {

AddColumnCmd::AddColumnCmd(NotNull<MapDocument*> document)
    : MergeableMapCommand{document, "Add Column(s)"}
{}

void AddColumnCmd::Undo()
{
  auto& map = GetMap();
  for (int i = 0; i < GetAmount(); ++i)
  {
    map.RemoveColumn();
  }
}

void AddColumnCmd::Redo()
{
  auto& map = GetMap();
  for (int i = 0; i < GetAmount(); ++i)
  {
    map.AddColumn(empty_tile);
  }
}

}  // namespace Tactile
