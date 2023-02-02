/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "renderer.hpp"

#include <algorithm>  // min, max
#include <cmath>      // fmod

#include <glm/common.hpp>
#include <imgui.h>

#include "core/layer/group_layer.hpp"
#include "core/layer/object.hpp"
#include "core/layer/object_layer.hpp"
#include "core/layer/tile_layer.hpp"
#include "core/map.hpp"
#include "core/tile/tileset.hpp"
#include "core/tile/tileset_bundle.hpp"
#include "model/settings.hpp"
#include "ui/conversions.hpp"
#include "ui/render/primitives.hpp"

namespace tactile {
namespace {

inline constexpr Color kActiveObjectColor {0xFF, 0xFF, 0x00, 0xFF};

[[nodiscard]] auto convert_bounds_to_rect(const CanvasInfo& canvas) -> Float4
{
  const auto pos = canvas.origin + (canvas.bounds.begin.as_vec2f() * canvas.grid_size);
  const auto size =
      (canvas.bounds.end - canvas.bounds.begin).as_vec2f() * canvas.grid_size;
  return {pos.x, pos.y, size.x, size.y};
}

[[nodiscard]] auto determine_canvas_render_bounds(const Float2& tl,
                                                  const Float2& br,
                                                  const Float2& origin,
                                                  const Float2& grid_size,
                                                  const float rows,
                                                  const float cols) -> Region
{
  const auto begin = (tl - origin) / grid_size;
  const auto end = (br - origin) / grid_size;

  const auto begin_row = std::max(0, static_cast<int32>(begin.y));
  const auto begin_col = std::max(0, static_cast<int32>(begin.x));

  const auto end_row = static_cast<int32>(std::min(rows, end.y + 1));
  const auto end_col = static_cast<int32>(std::min(cols, end.x + 1));

  Region bounds;

  bounds.begin = {begin_row, begin_col};
  bounds.end = {end_row, end_col};

  return bounds;
}

[[nodiscard]] auto create_canvas_info(const Viewport& viewport,
                                      const Float2& logical_tile_size,
                                      const TileExtent& extent) -> CanvasInfo
{
  CanvasInfo canvas;

  canvas.top_left = ui::to_vec(ImGui::GetCursorScreenPos());
  canvas.bottom_right = canvas.top_left + ui::to_vec(ImGui::GetContentRegionAvail());
  canvas.size = canvas.bottom_right - canvas.top_left;

  canvas.origin = canvas.top_left + viewport.get_offset();

  canvas.tile_size = logical_tile_size;
  canvas.grid_size = viewport.tile_size();
  canvas.ratio = canvas.grid_size / canvas.tile_size;

  const auto tiles_in_viewport = canvas.size / canvas.grid_size;
  canvas.tiles_in_viewport_x = static_cast<int32>(tiles_in_viewport.x) + 1;
  canvas.tiles_in_viewport_y = static_cast<int32>(tiles_in_viewport.y) + 1;

  canvas.row_count = static_cast<float>(extent.rows);
  canvas.col_count = static_cast<float>(extent.cols);
  canvas.content_size = Float2 {canvas.col_count, canvas.row_count} * canvas.grid_size;

  canvas.bounds = determine_canvas_render_bounds(canvas.top_left,
                                                 canvas.bottom_right,
                                                 canvas.origin,
                                                 canvas.grid_size,
                                                 canvas.row_count,
                                                 canvas.col_count);

  return canvas;
}

[[nodiscard]] auto create_canvas_info(const Viewport& viewport, const Map& map)
{
  return create_canvas_info(viewport, map.get_tile_size(), map.get_extent());
}

[[nodiscard]] auto create_canvas_info(const Viewport& viewport, const Tileset& tileset)
{
  const TileExtent extent {static_cast<usize>(tileset.row_count()),
                           static_cast<usize>(tileset.column_count())};
  return create_canvas_info(viewport, tileset.tile_size(), extent);
}

}  // namespace

Renderer::Renderer(const CanvasInfo& canvas_info)
    : mCanvas {canvas_info},
      mViewportRect {convert_bounds_to_rect(mCanvas)}
{
}

Renderer::Renderer(const Viewport& viewport, const Map& map)
    : Renderer {create_canvas_info(viewport, map)}
{
}

Renderer::Renderer(const Viewport& viewport, const Tileset& tileset)
    : Renderer {create_canvas_info(viewport, tileset)}
{
}

void Renderer::push_clip() const
{
  auto* list = ImGui::GetWindowDrawList();
  list->PushClipRect(ui::from_vec(mCanvas.top_left),
                     ui::from_vec(mCanvas.bottom_right),
                     true);
}

void Renderer::pop_clip() const
{
  auto* list = ImGui::GetWindowDrawList();
  list->PopClipRect();
}

void Renderer::clear(const Color& color) const
{
  fill_rect(mCanvas.top_left, mCanvas.size, color);
}

void Renderer::render_infinite_grid(const Color& color) const
{
  const auto origin_tile_pos = glm::floor(mCanvas.top_left / mCanvas.grid_size);
  const auto origin_col = static_cast<int32>(origin_tile_pos.x);
  const auto origin_row = static_cast<int32>(origin_tile_pos.y);

  const auto begin_row = origin_row - 1;
  const auto begin_col = origin_col - 1;
  const auto end_row = origin_row + mCanvas.tiles_in_viewport_y + 1;
  const auto end_col = origin_col + mCanvas.tiles_in_viewport_x + 1;

  // This offset ensures that the rendered grid is aligned over the underlying grid
  const ImVec2 offset {std::fmod(mCanvas.origin.x, mCanvas.grid_size.x),
                       std::fmod(mCanvas.origin.y, mCanvas.grid_size.y)};

  const auto end_x = (static_cast<float>(end_col) * mCanvas.grid_size.x) + offset.x;
  const auto end_y = (static_cast<float>(end_row) * mCanvas.grid_size.y) + offset.y;

  auto* list = ImGui::GetWindowDrawList();
  for (auto row = begin_row; row < end_row; ++row) {
    const auto row_y = (static_cast<float>(row) * mCanvas.grid_size.y) + offset.y;
    list->AddLine(ImVec2 {0, row_y}, ImVec2 {end_x, row_y}, ui::to_u32(color));
  }

  for (auto col = begin_col; col < end_col; ++col) {
    const auto col_x = (static_cast<float>(col) * mCanvas.grid_size.x) + offset.x;
    list->AddLine(ImVec2 {col_x, 0}, ImVec2 {col_x, end_y}, ui::to_u32(color));
  }
}

void Renderer::render_translated_grid(const Color& color) const
{
  const auto begin_row = mCanvas.bounds.begin.row();
  const auto begin_col = mCanvas.bounds.begin.col();
  const auto end_row = mCanvas.bounds.end.row();
  const auto end_col = mCanvas.bounds.end.col();

  for (auto row = begin_row; row < end_row; ++row) {
    for (auto col = begin_col; col < end_col; ++col) {
      draw_rect(translate(from_matrix_to_absolute(TilePos {row, col})),
                mCanvas.grid_size,
                color);
    }
  }
}

void Renderer::render_outline(const Color& color) const
{
  draw_rect(mCanvas.origin, mCanvas.content_size, color);
}

void Renderer::render_tile(const Tileset& tileset,
                           const TilePos& tile_position,
                           const Float2& position,
                           const float opacity) const
{
  const auto& uv_size = tileset.uv_size();

  const auto uv_min = tile_position.as_vec2f() * uv_size;
  const auto uv_max = uv_min + uv_size;

  render_image(tileset.texture(),
               position,
               mCanvas.grid_size,
               uv_min,
               uv_max,
               ui::opacity_cast(opacity));
}

void Renderer::render_tile(const Tileset& tileset,
                           const TileIndex tile_index,
                           const TilePos& pos,
                           const float opacity) const
{
  const auto tileset_pos = TilePos::from_index(tile_index, tileset.column_count());

  const auto absolute_position = from_matrix_to_absolute(pos);
  const auto rendered_position = translate(absolute_position);

  const auto& uv_size = tileset.uv_size();
  const auto uv_min = tileset_pos.as_vec2f() * uv_size;
  const auto uv_max = uv_min + uv_size;

  render_image(tileset.texture(),
               rendered_position,
               mCanvas.grid_size,
               uv_min,
               uv_max,
               ui::opacity_cast(opacity));
}

void Renderer::render_tile(const Map& map,
                           const TileID tile_id,
                           const TilePos& pos,
                           const float opacity) const
{
  TACTILE_ASSERT(tile_id != kEmptyTile);
  const auto& tileset_bundle = map.get_tileset_bundle();

  if (const auto tileset_id = tileset_bundle.find_tileset(tile_id)) {
    const auto& tileset_ref = tileset_bundle.get_tileset_ref(*tileset_id);
    const auto& tileset = tileset_ref.get_tileset();

    const auto tile_index = tileset.appearance_of(tileset_ref.to_index(tile_id));
    render_tile(tileset, tile_index, pos, opacity);
  }
}

void Renderer::render_tile_layer(const Map& map,
                                 const TileLayer& layer,
                                 const float parent_opacity) const
{
  const auto tile_opacity = parent_opacity * layer.get_opacity();

  const auto begin_row = mCanvas.bounds.begin.row();
  const auto begin_col = mCanvas.bounds.begin.col();
  const auto end_row = mCanvas.bounds.end.row();
  const auto end_col = mCanvas.bounds.end.col();

  for (auto row = begin_row; row < end_row; ++row) {
    for (auto col = begin_col; col < end_col; ++col) {
      const TilePos pos {row, col};
      const auto tile_id = layer.tile_at(pos);

      if (tile_id.has_value() && tile_id != kEmptyTile) {
        render_tile(map, *tile_id, pos, tile_opacity);
      }
    }
  }
}

void Renderer::render_object_layer(const ObjectLayer& layer,
                                   const float parent_opacity) const
{
  const auto opacity = parent_opacity * layer.get_opacity();
  const Color object_color {0xFF, 0, 0, static_cast<uint8>(255 * opacity)};

  for (const auto& [id, object]: layer) {
    render_object(*object, object_color);
  }
}

void Renderer::render_object(const Object& object, const Color& color) const
{
  const auto rendered_position = object.get_pos() * mCanvas.ratio;

  switch (object.get_type()) {
    case ObjectType::Point:
      render_point_object(object, rendered_position, color);
      break;

    case ObjectType::Ellipse:
      render_ellipse_object(object, rendered_position, color);
      break;

    case ObjectType::Rect:
      render_rectangle_object(object, rendered_position, color);
      break;
  }
}

void Renderer::render_point_object(const Object& object,
                                   const Float2& position,
                                   const Color& color) const
{
  TACTILE_ASSERT(object.is_point());

  const auto translated_position = translate(position);
  const float radius = std::min(mCanvas.grid_size.x / 4.0f, 6.0f);

  if (is_within_viewport(translated_position)) {
    draw_shadowed_circle(translated_position, radius, color, 2.0f);

    const auto& name = object.get_ctx().name();
    if (!name.empty()) {
      const auto text_size = ImGui::CalcTextSize(name.c_str());

      if (text_size.x <= mCanvas.grid_size.x) {
        const auto text_x = position.x - (text_size.x / 2.0f);
        const auto text_y = position.y + radius + 4.0f;

        render_text(name.c_str(), translate(Float2 {text_x, text_y}), kWhite);
      }
    }
  }
}

void Renderer::render_rectangle_object(const Object& object,
                                       const Float2& position,
                                       const Color& color) const
{
  TACTILE_ASSERT(object.is_rect());

  const auto translated_position = translate(position);
  const auto rendered_size = object.get_size() * mCanvas.ratio;

  if (is_intersecting_viewport(translated_position, rendered_size)) {
    draw_shadowed_rect(translated_position, rendered_size, color, 2.0f);

    const auto& name = object.get_ctx().name();
    if (!name.empty()) {
      const auto text_size = ImGui::CalcTextSize(name.c_str());

      if (text_size.x <= rendered_size.x) {
        const auto text_x = (rendered_size.x - text_size.x) / 2.0f;

        render_text(name.c_str(),
                    translate(position + Float2 {text_x, rendered_size.y + 4.0f}),
                    kWhite);
      }
    }
  }
}

void Renderer::render_ellipse_object(const Object& object,
                                     const Float2& position,
                                     const Color& color) const
{
  TACTILE_ASSERT(object.is_ellipse());

  const auto rendered_size = object.get_size() * mCanvas.ratio;

  const auto radius = Float2 {0.5f, 0.5f} * rendered_size;
  const auto center = position + radius;

  draw_shadowed_ellipse(translate(center), radius, color, 2.0f);

  const auto& name = object.get_ctx().name();
  if (!name.empty()) {
    const auto text_size = ImGui::CalcTextSize(name.c_str());

    if (text_size.x <= radius.x) {
      const auto text_x = center.x - (text_size.x / 2.0f);
      const auto text_y = center.y + (text_size.y / 2.0f) + (radius.y);

      render_text(name.c_str(), translate(Float2 {text_x, text_y}), kWhite);
    }
  }
}

void Renderer::render_map(const Map& map) const
{
  const auto& settings = get_settings();

  render_layers(map);

  if (const auto active_layer_id = map.get_active_layer_id()) {
    const auto* object_layer =
        map.get_invisible_root().find_object_layer(*active_layer_id);
    if (object_layer) {
      if (const auto object_id = object_layer->active_object_id()) {
        const auto& object = object_layer->get_object(*object_id);
        render_object(object, kActiveObjectColor);
      }
    }
  }

  if (settings.test_flag(SETTINGS_SHOW_GRID_BIT)) {
    render_infinite_grid(settings.get_grid_color());
  }

  render_outline(ui::to_color(ImGui::GetStyle().Colors[ImGuiCol_HeaderActive]));
}

void Renderer::render_layers(const Map& map) const
{
  const auto& settings = get_settings();

  const auto& root = map.get_invisible_root();
  const auto active_layer_id = map.get_active_layer_id();

  root.each([&](const Layer& layer) {
    if (!layer.is_visible()) {
      return;
    }

    const auto parent_id = layer.get_parent();
    const auto* parent_layer = parent_id ? root.find_group_layer(*parent_id) : nullptr;

    if (parent_layer && !parent_layer->is_visible()) {
      return;
    }

    const auto parent_opacity = parent_layer ? parent_layer->get_opacity() : 1.0f;
    if (settings.test_flag(SETTINGS_HIGHLIGHT_ACTIVE_LAYER_BIT)) {
      render_layer(map, layer, (active_layer_id == layer.get_uuid()) ? 1.0f : 0.5f);
    }
    else {
      render_layer(map, layer, parent_opacity * layer.get_opacity());
    }
  });
}

void Renderer::render_layer(const Map& map,
                            const Layer& layer,
                            float parent_opacity) const
{
  const auto type = layer.get_type();
  if (type == LayerType::TileLayer) {
    const auto& tile_layer = dynamic_cast<const TileLayer&>(layer);
    render_tile_layer(map, tile_layer, parent_opacity);
  }
  else if (type == LayerType::ObjectLayer) {
    const auto& object_layer = dynamic_cast<const ObjectLayer&>(layer);
    render_object_layer(object_layer, parent_opacity);
  }
}

void Renderer::render_tileset(const Tileset& tileset) const
{
  const auto& texture = tileset.texture();

  const auto rendered_position = translate(Float2 {0, 0});
  const auto rendered_size = Float2 {texture.get_size()} * mCanvas.ratio;

  render_image(texture, rendered_position, rendered_size);

  const auto& settings = get_settings();
  if (settings.test_flag(SETTINGS_SHOW_GRID_BIT)) {
    render_infinite_grid(settings.get_grid_color());
  }

  render_outline(ui::to_color(ImGui::GetStyle().Colors[ImGuiCol_HeaderActive]));
}

auto Renderer::from_matrix_to_absolute(const TilePos pos) const -> Float2
{
  return mCanvas.grid_size * pos.as_vec2f();
}

auto Renderer::is_intersecting_viewport(const Float2 position, const Float2 size) const
    -> bool
{
  const auto viewport_max_x = mViewportRect.x + mViewportRect.z;
  const auto viewport_max_y = mViewportRect.y + mViewportRect.w;

  const auto checked_max_x = position.x + size.x;
  const auto checked_max_y = position.y + size.y;

  return mViewportRect.x < checked_max_x &&  //
         mViewportRect.y < checked_max_y &&  //
         position.x < viewport_max_x &&      //
         position.y < viewport_max_y;
}

auto Renderer::is_within_viewport(const Float2 position) const -> bool
{
  const auto viewport_max_x = mViewportRect.x + mViewportRect.z;
  const auto viewport_max_y = mViewportRect.y + mViewportRect.w;

  return mViewportRect.x < position.x &&  //
         mViewportRect.y < position.y &&  //
         position.x < viewport_max_x &&   //
         position.y < viewport_max_y;
}

}  // namespace tactile