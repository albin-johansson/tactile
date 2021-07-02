#include "stamp_tool.hpp"

#include <cassert>  // assert

#include "core/model.hpp"

namespace Tactile {

StampTool::StampTool(NotNull<Model*> model) : AMouseTool{model}
{}

void StampTool::OnPressed(const MouseInfo& info)
{
  if (IsUsable() && info.button == cen::mouse_button::left)
  {
    const auto* tileset = GetTileset();
    assert(tileset);
    UpdateSequence(*tileset, info.mouse_position_in_map);
  }
}

void StampTool::OnDragged(const MouseInfo& info)
{
  if (IsUsable() && info.button == cen::mouse_button::left)
  {
    const auto* tileset = GetTileset();
    assert(tileset);
    UpdateSequence(*tileset, info.mouse_position_in_map);
  }
}

void StampTool::OnReleased(const MouseInfo& info)
{
  if (IsUsable() && info.button == cen::mouse_button::left)
  {
    // TODO document->AddStampSequence(std::move(mOldState),
    // std::move(mSequence));

    // Clearing the maps allows for them to be reused after being moved from
    mOldState.clear();
    mSequence.clear();
  }
}

auto StampTool::IsUsable() const -> bool
{
  const auto* document = GetDocument();
  return document && document->GetTilesets().GetSelection() &&
         document->GetMap().GetActiveLayerId();
}

void StampTool::UpdateSequence(const Tileset& tileset,
                               const MapPosition& origin)
{
  auto& map = GetDocument()->GetMap();

  auto* tileLayer = map.GetTileLayer(map.GetActiveLayerId().value());
  if (!tileLayer)
  {
    return;
  }

  const auto [topLeft, bottomRight] = tileset.GetSelection().value();

  const auto size = bottomRight - topLeft;
  const MapPosition offset = {size.GetRow() / 2_row, size.GetColumn() / 2_col};

  const auto rowCount = size.GetRow();
  const auto colCount = size.GetColumn();
  for (auto row = 0_row; row < rowCount; ++row)
  {
    for (auto col = 0_col; col < colCount; ++col)
    {
      const auto index = MapPosition{row, col};
      const auto tile = tileset.GetTile(topLeft + index);
      if (tile != empty_tile)
      {
        const auto pos = origin + index - offset;
        if (map.InBounds(pos))
        {
          if (!mOldState.contains(pos))
          {
            mOldState.emplace(pos, tileLayer->GetTile(pos).value());
          }
          mSequence.emplace_or_replace(pos, tile);

          tileLayer->SetTile(pos, tile);
        }
      }
    }
  }
}

}  // namespace Tactile
