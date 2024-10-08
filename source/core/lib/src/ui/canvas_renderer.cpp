// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/canvas_renderer.hpp"

#include <algorithm>  // clamp
#include <cmath>      // fmod

#include <imgui.h>

#include "tactile/base/numeric/vec_common.hpp"
#include "tactile/core/debug/assert.hpp"
#include "tactile/core/meta/color.hpp"
#include "tactile/core/ui/imgui_compat.hpp"
#include "tactile/core/ui/render/primitives.hpp"
#include "tactile/core/ui/viewport.hpp"

namespace tactile::ui {
namespace {

using VisibleRegion = CanvasRenderer::VisibleRegion;
using VisibleTileRegion = CanvasRenderer::VisibleTileRegion;
using RenderBounds = CanvasRenderer::RenderBounds;

[[nodiscard]]
auto _get_visible_region(const Float2& viewport_pos, const Float2& window_size)
    -> VisibleRegion
{
  VisibleRegion region {};

  region.begin = viewport_pos;
  region.end = viewport_pos + window_size;

  return region;
}

[[nodiscard]]
auto _get_visible_tiles(const VisibleRegion& visible_region, const Float2& tile_size)
    -> VisibleTileRegion
{
  VisibleTileRegion visible_tiles {};

  const auto tl = visible_region.begin / tile_size;
  const auto br = ceil(visible_region.end / tile_size);

  visible_tiles.begin.y = static_cast<Offset2D::value_type>(tl.y());
  visible_tiles.begin.x = static_cast<Offset2D::value_type>(tl.x());

  visible_tiles.end.y = static_cast<Offset2D::value_type>(br.y());
  visible_tiles.end.x = static_cast<Offset2D::value_type>(br.x());

  return visible_tiles;
}

[[nodiscard]]
auto _get_render_bounds(const VisibleTileRegion& tiles, const Extent2D& extent) -> RenderBounds
{
  RenderBounds bounds {};

  bounds.begin.y = std::clamp(saturate_cast<Index2D::value_type>(tiles.begin.y),
                              Index2D::value_type {0},
                              extent.rows);
  bounds.begin.x = std::clamp(saturate_cast<Index2D::value_type>(tiles.begin.x),
                              Extent2D::value_type {0},
                              extent.cols);

  bounds.end.y =
      std::clamp(saturate_cast<Index2D::value_type>(tiles.end.y), bounds.begin.y, extent.rows);
  bounds.end.x =
      std::clamp(saturate_cast<Index2D::value_type>(tiles.end.x), bounds.begin.x, extent.cols);

  return bounds;
}

}  // namespace

CanvasRenderer::CanvasRenderer(const Float2& canvas_tl,
                               const Float2& canvas_br,
                               const Extent2D& extent,
                               const Int2& tile_size,
                               const CViewport& viewport)
  : mExtent {extent},
    mViewportPos {viewport.pos},
    mCanvasTileSize {vec_cast<Float2>(tile_size) * viewport.scale},
    mWindowTL {canvas_tl},
    mWindowBR {canvas_br},
    mScale {viewport.scale},
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

void CanvasRenderer::clear_canvas(const UColor& color) const
{
  fill_rect(mWindowTL, mWindowBR, color);
}

void CanvasRenderer::draw_orthogonal_grid(const UColor& color) const
{
  const auto row_count =
      saturate_cast<Index2D::value_type>(mVisibleTiles.end.y - mVisibleTiles.begin.y);
  const auto col_count =
      saturate_cast<Index2D::value_type>(mVisibleTiles.end.x - mVisibleTiles.begin.x);

  const auto begin_pos = to_screen_pos(mVisibleRegion.begin);
  const auto begin_x = begin_pos.x();
  const auto begin_y = begin_pos.y();

  const auto end_pos = to_screen_pos(mVisibleRegion.end);
  const auto end_x = end_pos.x();
  const auto end_y = end_pos.y();

  // Used to align the rendered grid with the underlying tile grid.
  const auto grid_offset =
      apply2(get_content_position(), mCanvasTileSize, [](const float a, const float b) {
        return std::fmod(a, b);
      });
  const auto grid_offset_x = grid_offset.x();
  const auto grid_offset_y = grid_offset.y();

  const auto tile_width = mCanvasTileSize.x();
  const auto tile_height = mCanvasTileSize.y();

  const auto line_color = to_uint32_abgr(color);
  auto& draw_list = get_draw_list();

  // Draw horizontal lines.
  for (Index2D::value_type r = 0; r < row_count; ++r) {
    const auto row_offset = static_cast<float>(r) * tile_height;
    const auto line_y = begin_y + grid_offset_y + row_offset;
    draw_list.AddLine(ImVec2 {begin_x, line_y}, ImVec2 {end_x, line_y}, line_color);
  }

  // Draw vertical lines.
  for (Index2D::value_type c = 0; c < col_count; ++c) {
    const auto column_offset = static_cast<float>(c) * tile_width;
    const auto line_x = begin_x + grid_offset_x + column_offset;
    draw_list.AddLine(ImVec2 {line_x, begin_y}, ImVec2 {line_x, end_y}, line_color);
  }
}

void CanvasRenderer::draw_tile_outline(const Float2& world_pos, const UColor& color) const
{
  const auto tile_index = floor(world_pos / mCanvasTileSize);
  const auto tile_pos = tile_index * mCanvasTileSize;
  draw_rect(to_screen_pos(tile_pos), mCanvasTileSize, color);
}

auto CanvasRenderer::get_content_position() const noexcept -> Float2
{
  return -mViewportPos;
}

auto CanvasRenderer::get_canvas_position() const noexcept -> Float2
{
  return mWindowTL;
}

auto CanvasRenderer::get_canvas_size() const noexcept -> Float2
{
  return mWindowBR - mWindowTL;
}

auto CanvasRenderer::get_visible_region() const -> const VisibleRegion&
{
  return mVisibleRegion;
}

auto CanvasRenderer::get_visible_tiles() const noexcept -> const VisibleTileRegion&
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

auto CanvasRenderer::get_scale() const -> float
{
  return mScale;
}

auto CanvasRenderer::to_world_pos(const Float2& screen_pos) const noexcept -> Float2
{
  return screen_pos + mViewportPos;
}

auto CanvasRenderer::to_screen_pos(const Float2& world_pos) const noexcept -> Float2
{
  return world_pos - mViewportPos + mWindowTL;
}

auto CanvasRenderer::to_screen_pos(const Index2D& tile_pos) const noexcept -> Float2
{
  return to_screen_pos(to_float2(tile_pos) * mCanvasTileSize);
}

auto CanvasRenderer::get_draw_list() noexcept -> ImDrawList&
{
  auto* const draw_list = ImGui::GetWindowDrawList();
  TACTILE_ASSERT(draw_list != nullptr);
  return *draw_list;
}

}  // namespace tactile::ui
