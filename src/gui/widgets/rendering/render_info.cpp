#include "render_info.hpp"

#include <imgui_internal.h>

#include "core/map_document.hpp"
#include "gui/widgets/rendering/canvas.hpp"

namespace Tactile {

auto GetRenderInfo(const MapDocument& document, const CanvasInfo& canvas)
    -> RenderInfo
{
  RenderInfo info;

  const auto& map = document.GetMap();
  info.row_count = static_cast<float>(map.GetRowCount());
  info.col_count = static_cast<float>(map.GetColumnCount());

  const auto& viewport = document.GetViewportInfo();
  const auto offset = ImVec2{viewport.x_offset, viewport.y_offset};

  info.grid_size = {viewport.tile_width, viewport.tile_height};
  info.map_position = canvas.tl + offset;

  info.tile_size = {static_cast<float>(map.GetTileWidth()),
                    static_cast<float>(map.GetTileHeight())};

  info.bounds = GetRenderBounds(canvas, info);

  return info;
}

}  // namespace Tactile
