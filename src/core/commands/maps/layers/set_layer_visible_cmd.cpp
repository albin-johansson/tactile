#include "set_layer_visible_cmd.hpp"

#include "core/map_document.hpp"

namespace Tactile {

SetLayerVisibleCmd::SetLayerVisibleCmd(NotNull<MapDocument*> document,
                                       const layer_id id,
                                       const bool visible)
    : MapCommand{document, "Change Layer Visibility"}
    , mId{id}
    , mVisible{visible}
{}

void SetLayerVisibleCmd::Undo()
{
  auto& map = GetMap();
  map.SetVisible(mId, !mVisible);
}

void SetLayerVisibleCmd::Redo()
{
  auto& map = GetMap();
  map.SetVisible(mId, mVisible);
}

}  // namespace Tactile
