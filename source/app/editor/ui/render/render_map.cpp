/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
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

#include "render_map.hpp"

#include <imgui_internal.h>

#include "core/layer/group_layer.hpp"
#include "core/layer/object_layer.hpp"
#include "core/layer/tile_layer.hpp"
#include "editor/ui/render/graphics.hpp"
#include "editor/ui/render/render_object_layer.hpp"
#include "editor/ui/render/render_tile_layer.hpp"
#include "io/proto/preferences.hpp"
#include "misc/assert.hpp"
#include "model/document/map_document.hpp"

namespace tactile::ui {
namespace {

inline constexpr uint32 active_object_color = IM_COL32(0xFF, 0xFF, 0x00, 0xFF);
inline constexpr uint32 grid_color = IM_COL32(0xFF, 0xFF, 0xFF, 0x05);

void render_map_outline(Graphics& graphics)
{
  const auto& info = graphics.info();
  const auto& color = ImGui::GetStyle().Colors[ImGuiCol_HeaderActive];
  graphics.draw_rect(
      info.origin,
      info.contents_size,
      IM_COL32(color.x * 255.0f, color.y * 255.0f, color.z * 255.0f, 0xFF));
}

void highlight_active_object(Graphics& graphics,
                             const Map& map,
                             const UUID& active_layer_id)
{
  if (const auto* object_layer = map.invisible_root().as_object_layer(active_layer_id)) {
    if (const auto object_id = object_layer->active_object_id()) {
      const auto& object = object_layer->get_object(*object_id);
      render_object(graphics, object, active_object_color);
    }
  }
}

void render_layer(Graphics& graphics,
                  const Map& map,
                  const Layer* layer,
                  const float parent_opacity)
{
  TACTILE_ASSERT(layer);

  switch (layer->type()) {
    case LayerType::TileLayer:
      if (const auto* tile_layer = dynamic_cast<const TileLayer*>(layer)) {
        render_tile_layer(graphics, map, *tile_layer, parent_opacity);
      }
      break;
    case LayerType::ObjectLayer:
      if (const auto* object_layer = dynamic_cast<const ObjectLayer*>(layer)) {
        render_object_layer(graphics, *object_layer, parent_opacity);
      }
      break;

    default:
      break;
  }
}

void render_layers(Graphics& graphics, const Map& map)
{
  const auto& prefs = io::get_preferences();
  const auto& root = map.invisible_root();
  const auto active_layer_id = map.active_layer_id();

  root.each([&](const Layer* layer) {
    if (!layer->visible()) {
      return;
    }

    const auto parent_id = layer->parent();
    const auto* parent_layer = parent_id ? root.as_group_layer(*parent_id) : nullptr;

    if (parent_layer && !parent_layer->visible()) {
      return;
    }

    const auto parent_opacity = parent_layer ? parent_layer->opacity() : 1.0f;
    if (prefs.highlight_active_layer) {
      render_layer(graphics, map, layer, active_layer_id == layer->uuid() ? 1.0f : 0.5f);
    }
    else {
      render_layer(graphics, map, layer, parent_opacity * layer->opacity());
    }
  });
}

void render_grid(Graphics& graphics)
{
  const auto& info = graphics.info();

  const auto origin_tile_pos = ImFloor(info.origin / info.grid_size);
  const auto col_offset = static_cast<int32>(origin_tile_pos.x);
  const auto row_offset = static_cast<int32>(origin_tile_pos.y);

  const auto begin_row = -(row_offset + 1);
  const auto begin_col = -(col_offset + 1);
  const auto end_row = (info.tiles_in_viewport_y - row_offset) + 3;  // A little extra
  const auto end_col = (info.tiles_in_viewport_x - col_offset) + 1;

  for (auto row = begin_row; row < end_row; ++row) {
    for (auto col = begin_col; col < end_col; ++col) {
      const auto pos = graphics.from_matrix_to_absolute(row, col);
      graphics.draw_translated_rect(pos, graphics.viewport_tile_size(), grid_color);
    }
  }
}

}  // namespace

void render_map(Graphics& graphics, const MapDocument& document)
{
  const auto& prefs = io::get_preferences();
  const auto& map = document.get_map();

  render_layers(graphics, map);

  if (const auto active_layer_id = map.active_layer_id()) {
    highlight_active_object(graphics, map, *active_layer_id);
  }

  if (prefs.show_grid) {
    render_grid(graphics);
  }

  render_map_outline(graphics);
}

}  // namespace tactile::ui
