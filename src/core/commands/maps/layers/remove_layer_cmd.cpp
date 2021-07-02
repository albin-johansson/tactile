#include "remove_layer_cmd.hpp"

#include "core/map_document.hpp"

namespace Tactile {

RemoveLayerCmd::RemoveLayerCmd(NotNull<MapDocument*> document,
                               const layer_id id)
    : MapCommand{document, "Remove Layer"}
    , mId{id}
{}

void RemoveLayerCmd::Undo()
{
  auto& map = GetMap();
  map.AddLayer(mId, mLayer);

  // Make sure that the layer is returned to its original index
  while (map.IndexOf(mId).value() != mIndex)
  {
    if (map.IndexOf(mId).value() < mIndex)
    {
      map.MoveLayerDown(mId);
    }
    else
    {
      map.MoveLayerUp(mId);
    }
  }

  if (mWasLayerActive)
  {
    map.SelectLayer(mId);
  }
}

void RemoveLayerCmd::Redo()
{
  auto& map = GetMap();
  mWasLayerActive = map.GetActiveLayerId() == mId;
  mIndex = map.IndexOf(mId).value();
  mLayer = map.TakeLayer(mId);
}

}  // namespace Tactile
