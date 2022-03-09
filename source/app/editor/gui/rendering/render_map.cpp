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

#include "core/components/layer.hpp"
#include "core/components/object.hpp"
#include "core/components/parent.hpp"
#include "graphics.hpp"
#include "io/persistence/preferences.hpp"
#include "render_object_layer.hpp"
#include "render_tile_layer.hpp"

namespace tactile {
namespace {

void RenderLayer(graphics_ctx& graphics,
                 const entt::registry& registry,
                 const entt::entity layerEntity,
                 const comp::Layer& layer,
                 const float parentOpacity)
{
  if (layer.type == layer_type::tile_layer) {
    RenderTileLayer(graphics, registry, layerEntity, parentOpacity);
  }
  else if (layer.type == layer_type::object_layer) {
    RenderObjectLayer(graphics, registry, layerEntity, parentOpacity);
  }
}

}  // namespace

void RenderMap(graphics_ctx& graphics, const entt::registry& registry)
{
  for (auto&& [entity, node] : registry.view<comp::LayerTreeNode>().each()) {
    const auto& layer = registry.get<comp::Layer>(entity);
    const auto& parent = registry.get<comp::Parent>(entity);

    const auto* parentLayer = (parent.entity != entt::null)
                                  ? registry.try_get<comp::Layer>(parent.entity)
                                  : nullptr;
    const auto parentOpacity = parentLayer ? parentLayer->opacity : 1.0f;

    if (layer.visible) {
      if (!parentLayer || parentLayer->visible) {
        RenderLayer(graphics, registry, entity, layer, layer.opacity * parentOpacity);
      }
    }
  }

  if (const auto& activeObject = registry.ctx<comp::active_object>();
      activeObject.entity != entt::null) {
    RenderObject(graphics, registry, activeObject.entity, cen::colors::yellow);
  }

  graphics.set_line_thickness(1.0f);

  if (get_preferences().is_grid_visible()) {
    graphics.set_draw_color(cen::colors::white.with_alpha(20));
    graphics.render_translated_grid();
  }
}

}  // namespace tactile
