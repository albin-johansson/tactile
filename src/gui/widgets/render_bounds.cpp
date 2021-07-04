#include "render_bounds.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include <algorithm>  // min, max

#include "gui/canvas_info.hpp"

namespace Tactile {

[[nodiscard]] auto GetRenderBounds(const CanvasInfo& canvas,
                                   const ImVec2& originPos,
                                   const ImVec2& tileSize,
                                   const row_t nRows,
                                   const col_t nCols) -> RenderBounds
{
  const auto begin = (canvas.tl - originPos) / tileSize;
  const auto end = (canvas.br - originPos) / tileSize;

  const auto beginRow = std::max(0_row, AsRow(begin.y));
  const auto beginCol = std::max(0_col, AsColumn(begin.x));

  const auto endRow = std::min(nRows, AsRow(end.y) + 1_row);
  const auto endCol = std::min(nCols, AsColumn(end.x) + 1_col);

  RenderBounds bounds;
  bounds.begin = {beginRow, beginCol};
  bounds.end = {endRow, endCol};
  return bounds;
}

}  // namespace Tactile
