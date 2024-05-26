// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/canvas_renderer.hpp"

#include <algorithm>  // clamp
#include <cmath>      // fmod
#include <numbers>    // pi_v

#include <imgui.h>

#include "tactile/core/debug/assert.hpp"
#include "tactile/core/meta/color.hpp"
#include "tactile/core/numeric/vec_common.hpp"
#include "tactile/core/ui/common/window.hpp"
#include "tactile/core/ui/imgui_compat.hpp"
#include "tactile/core/ui/viewport.hpp"

namespace tactile::ui {
inline namespace renderer {

using VisibleRegion = CanvasRenderer::VisibleRegion;
using VisibleTileRegion = CanvasRenderer::VisibleTileRegion;
using RenderBounds = CanvasRenderer::RenderBounds;

inline constexpr float kHalfPi = std::numbers::pi_v<float> * 0.5f;
inline constexpr float kTau = std::numbers::pi_v<float> * 2.0f;

[[nodiscard]]
auto _get_visible_region(const Float2& viewport_pos,
                         const Float2& window_size) -> VisibleRegion
{
  VisibleRegion region {};

  region.begin = viewport_pos;
  region.end = viewport_pos + window_size;

  return region;
}

[[nodiscard]]
auto _get_visible_tiles(const VisibleRegion& visible_region,
                        const Float2& tile_size) -> VisibleTileRegion
{
  VisibleTileRegion visible_tiles {};

  const auto tl = visible_region.begin / tile_size;
  const auto br = ceil(visible_region.end / tile_size);

  visible_tiles.begin.row = static_cast<MatrixIndex::value_type>(tl.y());
  visible_tiles.begin.col = static_cast<MatrixIndex::value_type>(tl.x());

  visible_tiles.end.row = static_cast<MatrixIndex::value_type>(br.y());
  visible_tiles.end.col = static_cast<MatrixIndex::value_type>(br.x());

  return visible_tiles;
}

[[nodiscard]]
auto _get_render_bounds(const VisibleTileRegion& tiles,
                        const MatrixExtent& extent) -> RenderBounds
{
  RenderBounds bounds {};

  bounds.begin.row =
      std::clamp(tiles.begin.row, MatrixExtent::value_type {0}, extent.rows);
  bounds.begin.col =
      std::clamp(tiles.begin.col, MatrixExtent::value_type {0}, extent.cols);

  bounds.end.row = std::clamp(tiles.end.row, bounds.begin.row, extent.rows);
  bounds.end.col = std::clamp(tiles.end.col, bounds.begin.col, extent.cols);

  return bounds;
}

}  // namespace renderer

CanvasRenderer::CanvasRenderer(const Window& window,
                               const MatrixExtent& extent,
                               const Int2& tile_size,
                               const CViewport& viewport)
  : mExtent {extent},
    mViewportPos {viewport.pos},
    mCanvasTileSize {vec_cast<Float2>(tile_size) * viewport.scale},
    mWindowTL {window.get_pos()},
    mWindowBR {mWindowTL + window.get_size()},
    mVisibleRegion {_get_visible_region(mViewportPos, mWindowBR - mWindowTL)},
    mVisibleTiles {_get_visible_tiles(mVisibleRegion, mCanvasTileSize)},
    mRenderBounds {_get_render_bounds(mVisibleTiles, mExtent)}
{
  auto& draw_list = get_draw_list();
  draw_list.PushClipRect(to_imvec2(mWindowTL), to_imvec2(mWindowBR), false);
}

CanvasRenderer::~CanvasRenderer() noexcept
{
  auto& draw_list = get_draw_list();
  draw_list.PopClipRect();
}

void CanvasRenderer::draw_rect(const Float2& screen_pos,
                               const Float2& size,
                               const Color& color,
                               const float thickness)
{
  auto& draw_list = get_draw_list();
  draw_list.AddRect(to_imvec2(screen_pos),
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
  auto& draw_list = get_draw_list();
  draw_list.AddRectFilled(to_imvec2(screen_pos),
                          to_imvec2(screen_pos + size),
                          color.to_uint32_abgr());
}

void CanvasRenderer::draw_ngon(const Float2& screen_pos,
                               const float radius,
                               const uint32 color_mask,
                               const int segment_count,
                               const float thickness,
                               const float angle)
{
  if ((color_mask & IM_COL32_A_MASK) == 0 || segment_count <= 2) {
    return;
  }

  // We're drawing a closed shape, so we remove one of the segments/points.
  const auto segment_max =
      static_cast<float>(segment_count - 1) / static_cast<float>(segment_count);
  const auto angle_max = kTau * segment_max;

  auto& draw_list = get_draw_list();

  draw_list.PathArcTo(to_imvec2(screen_pos),
                      radius,
                      angle,
                      angle + angle_max,
                      segment_count - 1);
  draw_list.PathStroke(color_mask, ImDrawFlags_Closed, thickness);
}

void CanvasRenderer::draw_hexagon(const Float2& screen_pos,
                                  const float radius,
                                  const uint32 color_mask,
                                  const float thickness)
{
  draw_ngon(screen_pos, radius, color_mask, 6, thickness, kHalfPi);
}

void CanvasRenderer::clear_canvas(const Color& color) const
{
  fill_rect(mWindowTL, mWindowBR, color);
}

void CanvasRenderer::draw_orthogonal_grid(const Color& color) const
{
  const auto row_count = mVisibleTiles.end.row - mVisibleTiles.begin.row;
  const auto col_count = mVisibleTiles.end.col - mVisibleTiles.begin.col;

  const auto begin_pos = to_screen_pos(mVisibleRegion.begin);
  const auto begin_x = begin_pos.x();
  const auto begin_y = begin_pos.y();

  const auto end_pos = to_screen_pos(mVisibleRegion.end);
  const auto end_x = end_pos.x();
  const auto end_y = end_pos.y();

  // Used to align the rendered grid with the underlying tile grid.
  const auto grid_offset =
      apply2(get_content_position(),
             mCanvasTileSize,
             [](const float a, const float b) { return std::fmod(a, b); });
  const auto grid_offset_x = grid_offset.x();
  const auto grid_offset_y = grid_offset.y();

  const auto tile_width = mCanvasTileSize.x();
  const auto tile_height = mCanvasTileSize.y();

  const auto line_color = color.to_uint32_abgr();
  auto& draw_list = get_draw_list();

  // Draw horizontal lines.
  for (MatrixIndex::value_type r = 0; r < row_count + 1; ++r) {
    const auto row_offset = static_cast<float>(r) * tile_height;
    const auto line_y = begin_y + grid_offset_y + row_offset;
    draw_list.AddLine(ImVec2 {begin_x, line_y},
                      ImVec2 {end_x, line_y},
                      line_color);
  }

  // Draw vertical lines.
  for (MatrixIndex::value_type c = 0; c < col_count; ++c) {
    const auto column_offset = static_cast<float>(c) * tile_width;
    const auto line_x = begin_x + grid_offset_x + column_offset;
    draw_list.AddLine(ImVec2 {line_x, begin_y},
                      ImVec2 {line_x, end_y},
                      line_color);
  }
}

void CanvasRenderer::draw_tile_outline(const Float2& world_pos,
                                       const Color& color) const
{
  const auto tile_index = floor(world_pos / mCanvasTileSize);
  const auto tile_pos = tile_index * mCanvasTileSize;
  draw_rect(to_screen_pos(tile_pos), mCanvasTileSize, color);
}

auto CanvasRenderer::get_content_position() const noexcept -> Float2
{
  return -mViewportPos;
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

auto CanvasRenderer::get_draw_list() noexcept -> ImDrawList&
{
  auto* const draw_list = ImGui::GetWindowDrawList();
  TACTILE_ASSERT(draw_list != nullptr);
  return *draw_list;
}

}  // namespace tactile::ui
