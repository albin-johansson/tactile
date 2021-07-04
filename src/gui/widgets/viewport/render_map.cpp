#include "render_map.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "core/map/layers/layer_utils.hpp"
#include "core/map_document.hpp"
#include "gui/canvas_info.hpp"
#include "gui/widgets/render_bounds.hpp"
#include "gui/widgets/viewport/render_tile_layer.hpp"
#include "io/preferences.hpp"

namespace Tactile {
namespace {

inline constexpr auto out_border_color = IM_COL32(0xFF, 0xFF, 0xFF, 0xFF);
inline constexpr auto in_border_color = IM_COL32(0xFF, 0xFF, 0xFF, 50);

void RenderOutline(const int nRows,
                   const int nCols,
                   const ImVec2 offset,
                   const ImVec2 tileSize)
{
  const auto width = static_cast<float>(nCols) * tileSize.x;
  const auto height = static_cast<float>(nRows) * tileSize.y;
  ImGui::GetWindowDrawList()->AddRect(offset,
                                      offset + ImVec2{width, height},
                                      out_border_color);
}

void RenderGrid(const ImVec2& mapPos,
                const ImVec2& tileSize,
                const RenderBounds& bounds)
{
  auto* drawList = ImGui::GetWindowDrawList();
  const auto endRow = bounds.end.GetRow();
  const auto endCol = bounds.end.GetColumn();
  for (auto row = bounds.begin.GetRow(); row < endRow; ++row)
  {
    for (auto col = bounds.begin.GetColumn(); col < endCol; ++col)
    {
      const ImVec2 pos = {mapPos.x + (tileSize.x * static_cast<float>(col)),
                          mapPos.y + (tileSize.y * static_cast<float>(row))};
      drawList->AddRect(pos, pos + tileSize, in_border_color);
    }
  }
}

}  // namespace

void RenderMap(const MapDocument& document,
               const CanvasInfo& canvas,
               const ImVec2& mapPos,
               const ImVec2& tileSize)
{
  const auto nRows = document.GetRowCount();
  const auto nCols = document.GetColumnCount();
  const auto bounds = GetRenderBounds(canvas, mapPos, tileSize, nRows, nCols);

  for (const auto& [id, layer] : document.GetMap())
  {
    if (!layer->IsVisible())
    {
      continue;
    }

    if (const auto* tileLayer = AsTileLayer(layer))
    {
      RenderTileLayer(*tileLayer,
                      document.GetTilesets(),
                      mapPos,
                      tileSize,
                      bounds);
    }

    // TODO RenderObjectLayer()
  }

  if (Prefs::GetShowGrid())
  {
    RenderGrid(mapPos, tileSize, bounds);
  }

  RenderOutline(nRows, nCols, mapPos, tileSize);
}

}  // namespace Tactile
