#include "set_layer_opacity_cmd.hpp"

#include "core/map_document.hpp"

namespace Tactile {

SetLayerOpacityCmd::SetLayerOpacityCmd(NotNull<MapDocument*> document,
                                       const layer_id id,
                                       const float opacity)
    : MapCommand{document, "Set Layer Opacity"}
    , mId{id}
    , mOpacity{opacity}
{}

void SetLayerOpacityCmd::Undo()
{
  auto& map = GetMap();

  map.SetOpacity(mId, mPrevious.value());
  mPrevious.reset();
}

void SetLayerOpacityCmd::Redo()
{
  auto& map = GetMap();
  mPrevious = map.GetOpacity(mId);
  map.SetOpacity(mId, mOpacity);
}

auto SetLayerOpacityCmd::MergeWith(const ACommand& cmd) -> bool
{
  if (GetId() == cmd.GetId())
  {
    const auto& other = dynamic_cast<const SetLayerOpacityCmd&>(cmd);
    if (mId == other.mId)
    {
      mOpacity = other.mOpacity;
      return true;
    }
  }

  return false;
}

}  // namespace Tactile
