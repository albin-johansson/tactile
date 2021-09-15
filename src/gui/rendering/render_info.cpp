#include "render_info.hpp"

#include <imgui_internal.h>

#include "canvas.hpp"
#include "core/ctx/map.hpp"
#include "core/ctx/viewport.hpp"
#include "render_bounds.hpp"

namespace Tactile {
namespace {

[[nodiscard]] auto ConvertBoundsToRect(const RenderInfo& info) -> cen::frect
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

}  // namespace

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

  info.ratio = info.grid_size / info.tile_size;

  info.bounds = GetRenderBounds(canvas, info);
  info.bounds_rect = ConvertBoundsToRect(info);

  return info;
}

}  // namespace Tactile
