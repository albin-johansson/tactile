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

#include "core/documents/map_document.hpp"
#include "core/layers/group_layer.hpp"
#include "core/layers/layer_visitor.hpp"
#include "core/layers/object_layer.hpp"
#include "core/layers/tile_layer.hpp"
#include "editor/ui/rendering/graphics.hpp"
#include "editor/ui/rendering/render_object_layer.hpp"
#include "editor/ui/rendering/render_tile_layer.hpp"
#include "io/persistence/preferences.hpp"
#include "misc/assert.hpp"

namespace tactile::ui {
namespace {

void _render_layer(GraphicsCtx&        graphics,
                   const core::Map&    map,
                   const core::ILayer* layer,
                   const float         parentOpacity)
{
  TACTILE_ASSERT(layer);

  switch (layer->get_type()) {
    case LayerType::TileLayer:
      if (const auto* tileLayer = dynamic_cast<const core::TileLayer*>(layer)) {
        render_tile_layer(graphics, map, *tileLayer, parentOpacity);
      }
      break;
    case LayerType::ObjectLayer:
      if (const auto* objectLayer = dynamic_cast<const core::ObjectLayer*>(layer)) {
        render_object_layer(graphics, *objectLayer, parentOpacity);
      }
      break;

    default:
      break;
  }
}

}  // namespace

void render_map(GraphicsCtx& graphics, const MapDocument& document)
{
  const auto& prefs = io::get_preferences();
  const bool  highlightActiveLayer = prefs.highlight_active_layer();

  const auto& map = document.get_map();
  const auto  activeLayerId = map.active_layer_id();

  map.visit_layers([&](const core::ILayer* layer) {
    if (!layer->is_visible()) {
      return;
    }

    const auto parentId = layer->get_parent();

    if (parentId) {
      const auto* parentLayer = map.find_group_layer(*parentId);
      TACTILE_ASSERT(parentLayer != nullptr);

      if (!parentLayer->is_visible()) {
        return;
      }

      const auto parentOpacity = parentLayer ? parentLayer->get_opacity() : 1.0f;
      if (highlightActiveLayer) {
        _render_layer(graphics,
                      map,
                      layer,
                      activeLayerId == layer->get_uuid() ? 1.0f : 0.5f);
      }
      else {
        _render_layer(graphics, map, layer, parentOpacity * layer->get_opacity());
      }
    }
    else {
      _render_layer(graphics, map, layer, layer->get_opacity());
    }
  });

  if (activeLayerId) {
    if (const auto* objectLayer = map.find_object_layer(*activeLayerId)) {
      if (const auto objectId = objectLayer->active_object_id()) {
        const auto& object = objectLayer->get_object(*objectId);
        render_object(graphics, object, cen::colors::yellow);
      }
    }
  }

  graphics.set_line_thickness(1.0f);

  if (prefs.is_grid_visible()) {
    graphics.set_draw_color(cen::colors::white.with_alpha(20));
    graphics.render_translated_grid();
  }
}

}  // namespace tactile::ui
