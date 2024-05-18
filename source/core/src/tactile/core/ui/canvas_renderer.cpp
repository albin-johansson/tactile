// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/canvas_renderer.hpp"

#include <algorithm>  // clamp
#include <cmath>      // fmod

#include <imgui.h>

#include "tactile/core/debug/assert.hpp"
#include "tactile/core/entity/registry.hpp"
#include "tactile/core/meta/color.hpp"
#include "tactile/core/numeric/vec_common.hpp"
#include "tactile/core/ui/common/window.hpp"
#include "tactile/core/ui/imgui_compat.hpp"
#include "tactile/core/ui/viewport.hpp"
#include "tactile/core/util/lookup.hpp"

namespace tactile::ui {

CanvasRenderer::CanvasRenderer(const Window& window,
                               const MatrixExtent& extent,
                               const Int2& tile_size,
                               const CViewport& viewport)
  : mCanvasScale {viewport.scale},
    mExtent {extent},
    mViewportPos {viewport.pos},
    mCanvasTileSize {vec_cast<Float2>(tile_size) * viewport.scale},
    mWindowTL {window.get_pos()},
    mWindowBR {mWindowTL + window.get_size()},
    mVisibleRegion {_compute_visible_region()},
    mVisibleTiles {_compute_visible_tiles()},
    mRenderBounds {_compute_render_bounds()}
{
  auto* draw_list = ImGui::GetWindowDrawList();
  draw_list->PushClipRect(to_imvec2(mWindowTL), to_imvec2(mWindowBR), false);
}

CanvasRenderer::~CanvasRenderer() noexcept
{
  auto* draw_list = ImGui::GetWindowDrawList();
  draw_list->PopClipRect();
}

void CanvasRenderer::draw_rect(const Float2& screen_pos,
                               const Float2& size,
                               const Color& color,
                               const float thickness)
{
  auto* draw_list = ImGui::GetWindowDrawList();
  draw_list->AddRect(to_imvec2(screen_pos),
                     to_imvec2(screen_pos + size),
                     color.to_uint32_abgr(),
                     0.0f,
                     ImDrawFlags_None,
                     thickness);
}

void CanvasRenderer::fill_rect(const Float2& screen_pos,
                               const Float2& size,
                               const Color& color)
{
  auto* draw_list = ImGui::GetWindowDrawList();
  draw_list->AddRectFilled(to_imvec2(screen_pos),
                           to_imvec2(screen_pos + size),
                           color.to_uint32_abgr());
}

void CanvasRenderer::clear_canvas(const Color& color) const
{
  fill_rect(mWindowTL, mWindowBR, color);
}

void CanvasRenderer::draw_orthogonal_grid(const Color& color) const
{
  const auto row_count = mVisibleTiles.end.row - mVisibleTiles.begin.row;
  const auto col_count = mVisibleTiles.end.col - mVisibleTiles.begin.col;

  const auto begin_pos = to_screen_pos(mViewportPos);
  const auto begin_x = begin_pos.x();
  const auto begin_y = begin_pos.y();

  const auto end_pos = begin_pos + (mVisibleRegion.end - mVisibleRegion.begin);
  const auto end_x = end_pos.x();
  const auto end_y = end_pos.y();

  const auto content_origin_pos = to_screen_pos(Float2 {0, 0});

  // Used to align the rendered grid with the underlying tile grid.
  const Float2 grid_offset {
    std::fmod(content_origin_pos.x(), mCanvasTileSize.x()),
    std::fmod(content_origin_pos.y(), mCanvasTileSize.y()),
  };
  const auto grid_offset_x = grid_offset.x();
  const auto grid_offset_y = grid_offset.y();

  const auto tile_width = mCanvasTileSize.x();
  const auto tile_height = mCanvasTileSize.y();

  const auto line_color = color.to_uint32_abgr();
  auto* draw_list = ImGui::GetWindowDrawList();

  // Draw horizontal lines.
  for (ssize r = 0; r < row_count + 1; ++r) {
    const auto y = (static_cast<float>(r) * tile_height) + grid_offset_y;
    draw_list->AddLine(ImVec2 {begin_x, y}, ImVec2 {end_x, y}, line_color);
  }

  // Draw vertical lines.
  for (ssize c = 0; c < col_count; ++c) {
    const auto x = (static_cast<float>(c) * tile_width) + grid_offset_x;
    draw_list->AddLine(ImVec2 {x, begin_y}, ImVec2 {x, end_y}, line_color);
  }
}

void CanvasRenderer::draw_tile_outline(const Float2& world_pos,
                                       const Color& color) const
{
  const auto tile_index = floor(world_pos / mCanvasTileSize);
  const auto tile_pos = tile_index * mCanvasTileSize;
  draw_rect(to_screen_pos(tile_pos), mCanvasTileSize, color);
}

auto CanvasRenderer::get_visible_region() const -> const VisibleRegion&
{
  return mVisibleRegion;
}

auto CanvasRenderer::get_visible_tiles() const noexcept
    -> const VisibleTileRegion&
{
  return mVisibleTiles;
}

auto CanvasRenderer::get_render_bounds() const -> const RenderBounds&
{
  return mRenderBounds;
}

auto CanvasRenderer::get_canvas_tile_size() const -> Float2
{
  return mCanvasTileSize;
}

auto CanvasRenderer::to_world_pos(const Float2& screen_pos) const noexcept
    -> Float2
{
  return screen_pos + mViewportPos;
}

auto CanvasRenderer::to_screen_pos(const Float2& world_pos) const noexcept
    -> Float2
{
  return world_pos - mViewportPos + mWindowTL;
}

auto CanvasRenderer::_compute_visible_region() const -> VisibleRegion
{
  VisibleRegion region {};

  const auto window_size = mWindowBR - mWindowTL;
  region.begin = mViewportPos;
  region.end = mViewportPos + window_size;

  return region;
}

auto CanvasRenderer::_compute_visible_tiles() const -> VisibleTileRegion
{
  VisibleTileRegion visible_tiles {};

  const auto& region = mVisibleRegion;

  const auto tl = region.begin / mCanvasTileSize;
  const auto br = ceil(region.end / mCanvasTileSize);

  visible_tiles.begin.row = static_cast<ssize>(tl.y());
  visible_tiles.begin.col = static_cast<ssize>(tl.x());

  visible_tiles.end.row = static_cast<ssize>(br.y());
  visible_tiles.end.col = static_cast<ssize>(br.x());

  return visible_tiles;
}

auto CanvasRenderer::_compute_render_bounds() const -> RenderBounds
{
  RenderBounds bounds {};

  const auto& visible = mVisibleTiles;

  bounds.begin.row = std::clamp(visible.begin.row, ssize {0}, mExtent.rows);
  bounds.begin.col = std::clamp(visible.begin.col, ssize {0}, mExtent.cols);

  bounds.end.row = std::clamp(visible.end.row, bounds.begin.row, mExtent.rows);
  bounds.end.col = std::clamp(visible.end.col, bounds.begin.col, mExtent.cols);

  return bounds;
}

}  // namespace tactile::ui
