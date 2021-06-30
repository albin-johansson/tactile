#include "render_map.hpp"

#include "core/map_document.hpp"
#include "imgui.h"
#include "imgui_internal.h"

namespace Tactile {
namespace {

inline constexpr auto border_color = IM_COL32(0x10, 0x10, 0x10, 0xFF);

void RenderOutline(ImDrawList* drawList,
                   const int nRows,
                   const int nCols,
                   const ImVec2 offset,
                   const ImVec2 tileSize)
{
  const auto width = static_cast<float>(nCols) * tileSize.x;
  const auto height = static_cast<float>(nRows) * tileSize.y;

  drawList->AddLine(offset, {offset.x + width, offset.y}, border_color);
  drawList->AddLine(offset, {offset.x, offset.y + height}, border_color);

  drawList->AddLine({offset.x, offset.y + height},
                    {offset.x + width, offset.y + height},
                    border_color);
  drawList->AddLine({offset.x + width, offset.y},
                    {offset.x + width, offset.y + height},
                    border_color);
}

}  // namespace

void RenderMap(const MapDocument& document,
               ImDrawList* drawList,
               const ImVec2 mapPos,
               const ImVec2 tileSize)
{
  const auto nRows = document.GetRowCount();
  const auto nCols = document.GetColumnCount();
  for (auto row = 0; row < nRows; ++row)
  {
    for (auto col = 0; col < nCols; ++col)
    {
      const ImVec2 pos = {mapPos.x + (tileSize.x * static_cast<float>(col)),
                          mapPos.y + (tileSize.y * static_cast<float>(row))};
      drawList->AddRect(pos, pos + tileSize, border_color);
    }
  }

  RenderOutline(drawList, nRows, nCols, mapPos, tileSize);
}

}  // namespace Tactile
