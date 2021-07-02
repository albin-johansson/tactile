#include "bucket_tool.hpp"

#include "core/map/layers/layer_utils.hpp"
#include "core/map_document.hpp"

namespace Tactile {

BucketTool::BucketTool(NotNull<Model*> model) : AMouseTool{model}
{}

void BucketTool::OnPressed(const MouseInfo& info)
{
  if (info.button == cen::mouse_button::left)
  {
    if (auto* document = GetDocument())
    {
      const auto* tileset = GetTileset();
      if (tileset && tileset->IsSingleTileSelected())
      {
        auto& map = document->GetMap();
        auto* tileLayer = map.GetTileLayer(map.GetActiveLayerId().value());

        const auto selection = tileset->GetSelection()->top_left;
        const auto replacement = tileset->GetTile(selection);

        // TODO command
        std::vector<MapPosition> positions;
        tileLayer->Flood(info.mouse_position_in_map, replacement, positions);
      }
    }
  }
}

}  // namespace Tactile
