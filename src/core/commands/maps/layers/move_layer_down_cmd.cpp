#include "move_layer_down_cmd.hpp"

#include "core/map_document.hpp"

namespace Tactile {

MoveLayerDownCmd::MoveLayerDownCmd(NotNull<MapDocument*> document,
                                   const layer_id id)
    : MapCommand{document, "Move Layer Down"}
    , mId{id}
{}

void MoveLayerDownCmd::Undo()
{
  auto& map = GetMap();
  map.MoveLayerUp(mId);
}

void MoveLayerDownCmd::Redo()
{
  auto& map = GetMap();
  map.MoveLayerDown(mId);
}

}  // namespace Tactile
