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
#include "core/debug/assert.hpp"
#include "graphics/graphics.hpp"
#include "graphics/render_object_layer.hpp"
#include "graphics/render_tile_layer.hpp"
#include "io/proto/preferences.hpp"
#include "model/document/map_document.hpp"
#include "ui/conversions.hpp"

namespace tactile::ui {
namespace {

inline constexpr uint32 active_object_color = IM_COL32(0xFF, 0xFF, 0x00, 0xFF);

void highlight_active_object(Graphics& graphics,
                             const Map& map,
                             const UUID& active_layer_id)
{
  if (const auto* object_layer =
          map.invisible_root().find_object_layer(active_layer_id)) {
    if (const auto object_id = object_layer->active_object_id()) {
      const auto& object = object_layer->get_object(*object_id);
      render_object(graphics, object, active_object_color);
    }
  }
}

void render_layer(Graphics& graphics,
                  const Map& map,
                  const Layer& layer,
                  const float parent_opacity)
{
  const auto type = layer.get_type();
  if (type == LayerType::TileLayer) {
    const auto& tile_layer = dynamic_cast<const TileLayer&>(layer);
    render_tile_layer(graphics, map, tile_layer, parent_opacity);
  }
  else if (type == LayerType::ObjectLayer) {
    const auto& object_layer = dynamic_cast<const ObjectLayer&>(layer);
    render_object_layer(graphics, object_layer, parent_opacity);
  }
}

void render_layers(Graphics& graphics, const Map& map)
{
  const auto& prefs = io::get_preferences();
  const auto& root = map.invisible_root();
  const auto active_layer_id = map.active_layer_id();

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
    if (prefs.highlight_active_layer) {
      render_layer(graphics,
                   map,
                   layer,
                   active_layer_id == layer.get_uuid() ? 1.0f : 0.5f);
    }
    else {
      render_layer(graphics, map, layer, parent_opacity * layer.get_opacity());
    }
  });
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
    graphics.render_infinite_grid(to_u32(prefs.grid_color));
  }

  const auto& color = to_color(ImGui::GetStyle().Colors[ImGuiCol_HeaderActive]);
  graphics.outline_contents(to_u32(color));
}

}  // namespace tactile::ui
