#include "render_bounds.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include <algorithm>  // min, max

#include "canvas.hpp"
#include "render_info.hpp"

namespace Tactile {

auto GetRenderBounds(const CanvasInfo& canvas, const RenderInfo& info) -> Region
{
  const auto begin = (canvas.tl - info.map_position) / info.grid_size;
  const auto end = (canvas.br - info.map_position) / info.grid_size;

  const auto beginRow = std::max(0_row, AsRow(begin.y));
  const auto beginCol = std::max(0_col, AsColumn(begin.x));

  const auto endRow = std::min(AsRow(info.row_count), AsRow(end.y) + 1_row);
  const auto endCol = std::min(AsColumn(info.col_count), AsColumn(end.x) + 1_col);

  Region bounds;

  bounds.begin = {beginRow, beginCol};
  bounds.end = {endRow, endCol};

  return bounds;
}

auto ConvertBoundsToRect(const RenderInfo& info) -> cen::frect
{
  const auto begin = info.bounds.begin;

  const auto gridWidth = info.grid_size.x;
  const auto gridHeight = info.grid_size.y;

  const auto index = ImVec2{static_cast<float>(begin.GetColumn()),
                            static_cast<float>(begin.GetRow())};
  const auto pos = info.map_position + (index * info.grid_size);

  const auto size = info.bounds.end - info.bounds.begin;
  const auto width = static_cast<float>(size.GetColumn()) * gridWidth;
  const auto height = static_cast<float>(size.GetRow()) * gridHeight;

  return cen::frect{pos.x, pos.y, width, height};
}

}  // namespace Tactile
