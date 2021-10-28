#include "render_bounds.hpp"

#include <algorithm>  // min, max

#include <imgui.h>
#include <imgui_internal.h>

#include "canvas.hpp"
#include "render_info.hpp"

namespace Tactile {

auto GetRenderBounds(const CanvasInfo& canvas, const RenderInfo& info) -> Region
{
  const auto begin = (canvas.tl - info.map_position) / info.grid_size;
  const auto end = (canvas.br - info.map_position) / info.grid_size;

  const auto beginRow = std::max(0, static_cast<int32>(begin.y));
  const auto beginCol = std::max(0, static_cast<int32>(begin.x));

  const auto endRow = static_cast<int32>(std::min(info.row_count, end.y + 1));
  const auto endCol = static_cast<int32>(std::min(info.col_count, end.x + 1));

  Region bounds;

  bounds.begin = {beginRow, beginCol};
  bounds.end = {endRow, endCol};

  return bounds;
}

}  // namespace Tactile
