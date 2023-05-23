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

#include "tileset_view.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include "common/color.hpp"
#include "components/texture.hpp"
#include "core/map.hpp"
#include "core/tileset.hpp"
#include "io/proto/settings.hpp"
#include "model/event/tileset_events.hpp"
#include "model/event/viewport_events.hpp"
#include "model/systems/document_system.hpp"
#include "ui/conversions.hpp"
#include "ui/render/canvas.hpp"
#include "ui/render/canvas_renderer.hpp"
#include "ui/render/primitives.hpp"
#include "ui/widget/rubber_band.hpp"

namespace tactile::ui {
namespace {

inline constexpr Color kRubberBandColor {0, 0x44, 0xCC, 100};

void _update_viewport_offset(const Entity attached_tileset_entity,
                             const Viewport& viewport,
                             const Texture& texture,
                             const Float2& canvas_viewport_size,
                             Dispatcher& dispatcher)
{
  const Float2 texture_size = texture.size;
  const Float2 min_offset = canvas_viewport_size - texture_size;
  const Float2 max_offset {};

  if (!viewport.limits.has_value() || min_offset != viewport.limits->min_offset) {
    dispatcher.enqueue<UpdateTilesetViewportLimitsEvent>(attached_tileset_entity,
                                                         min_offset,
                                                         max_offset);
  }

  ImGui::InvisibleButton("##TilesetViewInvisibleButton",
                         as_imvec2(canvas_viewport_size),
                         ImGuiButtonFlags_MouseButtonLeft |
                             ImGuiButtonFlags_MouseButtonMiddle |
                             ImGuiButtonFlags_MouseButtonRight);

  // This has no effect when users use touchpads, but that is handled separately
  if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Middle)) {
    const auto& io = ImGui::GetIO();
    const Float2 delta {io.MouseDelta.x, io.MouseDelta.y};
    dispatcher.enqueue<OffsetTilesetViewportEvent>(attached_tileset_entity, delta);
  }
}

void _render_selection(const Region& selection,
                       const ImVec2& min,
                       const ImVec2& tile_size)
{
  const auto diff = selection.end - selection.begin;

  const auto origin = as_imvec2(selection.begin.as_vec2f()) * tile_size;
  const auto size = as_imvec2(diff.as_vec2f()) * tile_size;

  fill_rect(min + origin, size, kRubberBandColor);
}

}  // namespace

void update_attached_tileset_view(const Model& model,
                                  const Entity attached_tileset_entity,
                                  Dispatcher& dispatcher)
{
  const auto& settings = model.get<Settings>();

  const auto& attached_tileset = model.get<AttachedTileset>(attached_tileset_entity);
  const auto& tileset = model.get<Tileset>(attached_tileset.tileset);
  const auto& texture = model.get<Texture>(tileset.texture);
  const auto& viewport = model.get<Viewport>(attached_tileset_entity);

  const TileExtent tileset_extent {static_cast<usize>(tileset.row_count),
                                   static_cast<usize>(tileset.column_count)};
  const auto canvas = create_canvas_info(viewport, tileset.tile_size, tileset_extent);

  _update_viewport_offset(attached_tileset_entity,
                          viewport,
                          texture,
                          as_float2(canvas.size),
                          dispatcher);

  clear_canvas(canvas, settings.get_viewport_bg_color());

  const auto offset = as_imvec2(viewport.offset);
  const auto tile_size = as_imvec2(tileset.tile_size);

  if (const auto selection = ui_rubber_band(offset, tile_size)) {
    dispatcher.enqueue<SetTilesetSelectionEvent>(*selection);
  }

  push_scissor(canvas);

  const auto position = ImGui::GetWindowDrawList()->GetClipRectMin() + offset;
  render_image(texture.handle, position, as_imvec2(texture.size));

  if (attached_tileset.selection.has_value()) {
    _render_selection(*attached_tileset.selection, position, tile_size);
  }

  render_translated_grid(canvas, settings.get_grid_color());
  pop_scissor();
}

}  // namespace tactile::ui
