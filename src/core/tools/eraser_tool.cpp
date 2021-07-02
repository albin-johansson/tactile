#include "eraser_tool.hpp"

#include <cassert>  // assert

#include "core/map_document.hpp"

namespace Tactile {

EraserTool::EraserTool(NotNull<Model*> model) : AMouseTool{model}
{}

void EraserTool::OnPressed(const MouseInfo& info)
{
  if (IsUsable() && info.button == cen::mouse_button::left)
  {
    UpdateSequence(info.mouse_position_in_map);
  }
}

void EraserTool::OnDragged(const MouseInfo& info)
{
  if (IsUsable() && info.button == cen::mouse_button::left)
  {
    UpdateSequence(info.mouse_position_in_map);
  }
}

void EraserTool::OnReleased(const MouseInfo& info)
{
  if (IsUsable() && info.button == cen::mouse_button::left)
  {
    // TODO register command
    mOldState.clear();
  }
}

void EraserTool::UpdateSequence(const MapPosition& position)
{
  auto* document = GetDocument();
  assert(document);

  auto& map = document->GetMap();
  auto* tileLayer = map.GetTileLayer(map.GetActiveLayerId().value());
  assert(tileLayer);

  if (!mOldState.contains(position))
  {
    mOldState.emplace(position, tileLayer->GetTile(position).value());
  }

  tileLayer->SetTile(position, empty_tile);
}

auto EraserTool::IsUsable() const -> bool
{
  if (const auto* document = GetDocument())
  {
    const auto& map = document->GetMap();
    return map.GetActiveLayerId() && map.IsTileLayer(*map.GetActiveLayerId());
  }
  else
  {
    return false;
  }
}

}  // namespace Tactile
