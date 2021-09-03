#include "render_info.hpp"

#include <imgui_internal.h>

#include "canvas.hpp"
#include "core/map.hpp"
#include "core/viewport.hpp"
#include "render_bounds.hpp"

namespace Tactile {

auto GetRenderInfo(const entt::registry& registry, const CanvasInfo& canvas)
    -> RenderInfo
{
  RenderInfo info;

  const auto& map = registry.ctx<Map>();
  info.row_count = static_cast<float>(map.row_count);
  info.col_count = static_cast<float>(map.column_count);

  const auto& viewport = registry.ctx<Viewport>();
  const auto offset = ImVec2{viewport.x_offset, viewport.y_offset};

  info.grid_size = {viewport.tile_width, viewport.tile_height};
  info.map_position = canvas.tl + offset;

  info.tile_size = {static_cast<float>(map.tile_width),
                    static_cast<float>(map.tile_height)};

  info.bounds = GetRenderBounds(canvas, info);

  return info;
}

}  // namespace Tactile
