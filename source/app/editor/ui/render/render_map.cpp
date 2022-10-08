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

#include "core/document/map_document.hpp"
#include "core/layer/group_layer.hpp"
#include "core/layer/layer_visitor.hpp"
#include "core/layer/object_layer.hpp"
#include "core/layer/tile_layer.hpp"
#include "editor/ui/render/graphics.hpp"
#include "editor/ui/render/render_object_layer.hpp"
#include "editor/ui/render/render_tile_layer.hpp"
#include "io/proto/preferences.hpp"
#include "misc/assert.hpp"

namespace tactile::ui {
namespace {

void render_layer(Graphics& graphics,
                  const Map& map,
                  const Layer* layer,
                  const float parent_opacity)
{
  TACTILE_ASSERT(layer);

  switch (layer->get_type()) {
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

}  // namespace

void render_map(Graphics& graphics, const MapDocument& document)
{
  const auto& prefs = io::get_preferences();
  const auto& map = document.get_map();
  const auto& root = map.invisible_root();
  const auto active_layer_id = map.active_layer_id();

  root.each([&](const Layer* layer) {
    if (!layer->is_visible()) {
      return;
    }

    const auto parent_id = layer->get_parent();
    const auto* parent_layer = parent_id ? root.as_group_layer(*parent_id) : nullptr;

    if (parent_layer && !parent_layer->is_visible()) {
      return;
    }

    const auto parent_opacity = parent_layer ? parent_layer->get_opacity() : 1.0f;
    if (prefs.highlight_active_layer) {
      render_layer(graphics,
                   map,
                   layer,
                   active_layer_id == layer->get_uuid() ? 1.0f : 0.5f);
    }
    else {
      render_layer(graphics, map, layer, parent_opacity * layer->get_opacity());
    }
  });

  if (active_layer_id) {
    if (const auto* object_layer = root.as_object_layer(*active_layer_id)) {
      if (const auto object_id = object_layer->active_object_id()) {
        const auto& object = object_layer->get_object(*object_id);
        render_object(graphics, object, IM_COL32(0xFF, 0xFF, 0, 0xFF));
      }
    }
  }

  if (prefs.show_grid) {
    graphics.render_translated_grid(IM_COL32(0xFF, 0xFF, 0xFF, 20));
  }
}

}  // namespace tactile::ui
