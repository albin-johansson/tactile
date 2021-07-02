#include "select_layer_cmd.hpp"

#include "core/map_document.hpp"

namespace Tactile {

SelectLayerCmd::SelectLayerCmd(NotNull<MapDocument*> document,
                               const layer_id id)
    : MapCommand{document, "Select Layer"}
    , mId{id}
{}

void SelectLayerCmd::Undo()
{
  if (mPrevious)
  {
    auto& map = GetMap();
    map.SelectLayer(*mPrevious);
    mPrevious.reset();
  }
}

void SelectLayerCmd::Redo()
{
  auto& map = GetMap();

  mPrevious = map.GetActiveLayerId();
  map.SelectLayer(mId);
}

}  // namespace Tactile
