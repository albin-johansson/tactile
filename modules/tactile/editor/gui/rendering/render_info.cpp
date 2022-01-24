#include "render_info.hpp"

#include <algorithm>  // min, max

#include <imgui_internal.h>

#include "core/components/tileset.hpp"
#include "core/map.hpp"
#include "core/viewport.hpp"

namespace tactile {
namespace {

auto GetRenderBounds(const ImVec2& tl,
                     const ImVec2& br,
                     const ImVec2& origin,
                     const ImVec2& gridSize,
                     const float rows,
                     const float cols) -> Region
{
  const auto begin = (tl - origin) / gridSize;
  const auto end = (br - origin) / gridSize;

  const auto beginRow = std::max(0, static_cast<int32>(begin.y));
  const auto beginCol = std::max(0, static_cast<int32>(begin.x));

  const auto endRow = static_cast<int32>(std::min(rows, end.y + 1));
  const auto endCol = static_cast<int32>(std::min(cols, end.x + 1));

  Region bounds;

  bounds.begin = {beginRow, beginCol};
  bounds.end = {endRow, endCol};

  return bounds;
}

auto GetRenderInfo(const Viewport& viewport,
                   const ImVec2& logicalTileSize,
                   const int32 rows,
                   const int32 columns) -> RenderInfo
{
  RenderInfo info;

  info.canvas_tl = ImGui::GetCursorScreenPos();
  info.canvas_br = info.canvas_tl + ImGui::GetContentRegionAvail();

  info.origin = info.canvas_tl + ImVec2{viewport.x_offset, viewport.y_offset};

  info.tile_size = logicalTileSize;
  info.grid_size = {viewport.tile_width, viewport.tile_height};
  info.ratio = info.grid_size / info.tile_size;

  info.row_count = static_cast<float>(rows);
  info.col_count = static_cast<float>(columns);

  info.bounds = GetRenderBounds(info.canvas_tl,
                                info.canvas_br,
                                info.origin,
                                info.grid_size,
                                info.row_count,
                                info.col_count);

  return info;
}

}  // namespace

auto GetRenderInfo(const Viewport& viewport, const MapInfo& map) -> RenderInfo
{
  const ImVec2 tileSize{static_cast<float>(map.tile_width),
                        static_cast<float>(map.tile_height)};
  return GetRenderInfo(viewport,
                       tileSize,
                       static_cast<int32>(map.row_count),
                       static_cast<int32>(map.column_count));
}

auto GetRenderInfo(const Viewport& viewport, const Tileset& tileset) -> RenderInfo
{
  const ImVec2 tileSize{static_cast<float>(tileset.tile_width),
                        static_cast<float>(tileset.tile_height)};
  return GetRenderInfo(viewport, tileSize, tileset.row_count, tileset.column_count);
}

}  // namespace tactile
