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

#include "core/components/layers.hpp"
#include "core/components/objects.hpp"
#include "core/components/parent.hpp"
#include "core/systems/registry_system.hpp"
#include "graphics.hpp"
#include "io/persistence/preferences.hpp"
#include "render_object_layer.hpp"
#include "render_tile_layer.hpp"

namespace tactile {
namespace {

void _render_layer(GraphicsCtx& graphics,
                   const entt::registry& registry,
                   const entt::entity layerEntity,
                   const comp::Layer& layer,
                   const float parentOpacity)
{
  if (layer.type == LayerType::tile_layer) {
    render_tile_layer(graphics, registry, layerEntity, parentOpacity);
  }
  else if (layer.type == LayerType::object_layer) {
    render_object_layer(graphics, registry, layerEntity, parentOpacity);
  }
}

}  // namespace

void render_map(GraphicsCtx& graphics, const entt::registry& registry)
{
  const auto& prefs = get_preferences();
  const auto& activeLayer = registry.ctx<comp::ActiveLayer>();

  const bool highlightActiveLayer = prefs.highlight_active_layer();

  for (auto&& [entity, node] : registry.view<comp::LayerTreeNode>().each()) {
    const auto& layer = sys::checked_get<comp::Layer>(registry, entity);
    const auto& parent = sys::checked_get<comp::Parent>(registry, entity);

    const auto* parentLayer = (parent.entity != entt::null)
                                  ? registry.try_get<comp::Layer>(parent.entity)
                                  : nullptr;
    const auto parentOpacity = parentLayer ? parentLayer->opacity : 1.0f;

    if (layer.visible) {
      if (!parentLayer || parentLayer->visible) {
        if (highlightActiveLayer) {
          _render_layer(graphics,
                        registry,
                        entity,
                        layer,
                        activeLayer.entity == entity ? 1.0f : 0.5f);
        }
        else {
          _render_layer(graphics, registry, entity, layer, parentOpacity * layer.opacity);
        }
      }
    }
  }

  if (const auto& activeObject = registry.ctx<comp::ActiveObject>();
      activeObject.entity != entt::null) {
    render_object(graphics, registry, activeObject.entity, cen::colors::yellow);
  }

  graphics.set_line_thickness(1.0f);

  if (get_preferences().is_grid_visible()) {
    graphics.set_draw_color(cen::colors::white.with_alpha(20));
    graphics.render_translated_grid();
  }
}

}  // namespace tactile
