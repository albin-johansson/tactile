#include "move_layer_up_cmd.hpp"

#include "core/map_document.hpp"

namespace Tactile {

MoveLayerUpCmd::MoveLayerUpCmd(NotNull<MapDocument*> document, const layer_id id)
    : MapCommand{document, "Move Layer Up"}
    , mId{id}
{}

void MoveLayerUpCmd::Undo()
{
  auto& map = GetMap();
  map.MoveLayerDown(mId);
}

void MoveLayerUpCmd::Redo()
{
  auto& map = GetMap();
  map.MoveLayerUp(mId);
}

}  // namespace Tactile