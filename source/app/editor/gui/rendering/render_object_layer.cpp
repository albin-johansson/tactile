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

#include "render_object_layer.hpp"

#include <algorithm>  // min

#include <imgui.h>
#include <imgui_internal.h>

#include "core/components/attributes.hpp"
#include "core/components/layers.hpp"
#include "core/components/objects.hpp"
#include "graphics.hpp"
#include "misc/assert.hpp"

namespace tactile {
namespace {

void RenderPointObject(graphics_ctx& graphics,
                       const entt::registry& registry,
                       const entt::entity objectEntity,
                       const ImVec2& position,
                       const cen::color& color)
{
  const float radius = (std::min)(graphics.viewport_tile_size().x / 4.0f, 6.0f);

  const auto& object = registry.get<comp::Object>(objectEntity);
  TACTILE_ASSERT(object.type == ObjectType::point);

  if (graphics.is_within_translated_bounds(position)) {
    graphics.set_draw_color(color);
    graphics.set_line_thickness(2.0f);
    graphics.draw_translated_circle_with_shadow(position, radius);

    const auto& context = registry.get<comp::AttributeContext>(objectEntity);
    if (!context.name.empty()) {
      const auto* name = context.name.c_str();
      const auto textSize = ImGui::CalcTextSize(name);
      if (textSize.x <= graphics.viewport_tile_size().x) {
        const auto textX = position.x - (textSize.x / 2.0f);
        const auto textY = position.y + radius + 4.0f;

        graphics.set_draw_color(cen::colors::white);
        graphics.render_translated_text(name, {textX, textY});
      }
    }
  }
}

void RenderEllipseObject(graphics_ctx& graphics,
                         const entt::registry& registry,
                         const entt::entity objectEntity,
                         const ImVec2& position,
                         const cen::color& color)
{
  const auto& object = registry.get<comp::Object>(objectEntity);
  const auto& context = registry.get<comp::AttributeContext>(objectEntity);
  TACTILE_ASSERT(object.type == ObjectType::ellipse);

  const ImVec2 size = {object.width, object.height};

  const auto radius = ImVec2{0.5f, 0.5f} * size * graphics.tile_size_ratio();
  const auto center = position + radius;

  graphics.set_draw_color(color);
  graphics.set_line_thickness(2);
  graphics.draw_translated_ellipse_with_shadow(center, radius);

  if (!context.name.empty()) {
    const c_str text = context.name.c_str();
    const auto textSize = ImGui::CalcTextSize(text);
    if (textSize.x <= radius.x) {
      const auto textX = center.x - (textSize.x / 2.0f);
      const auto textY = center.y + (textSize.y / 2.0f) + (radius.y);

      graphics.set_draw_color(cen::colors::white);
      graphics.render_translated_text(text, {textX, textY});
    }
  }
}

void RenderRectangleObject(graphics_ctx& graphics,
                           const entt::registry& registry,
                           const entt::entity objectEntity,
                           const ImVec2& position,
                           const cen::color& color)
{
  const auto& object = registry.get<comp::Object>(objectEntity);
  TACTILE_ASSERT(object.type == ObjectType::rect);

  const auto size = ImVec2{object.width, object.height} * graphics.tile_size_ratio();

  if (graphics.is_intersecting_bounds(position, size)) {
    graphics.set_draw_color(color);
    graphics.set_line_thickness(2.0f);
    graphics.draw_translated_rect_with_shadow(position, size);

    const auto& context = registry.get<comp::AttributeContext>(objectEntity);
    if (!context.name.empty()) {
      const auto* name = context.name.c_str();
      const auto textSize = ImGui::CalcTextSize(name);
      if (textSize.x <= size.x) {
        const auto textX = (size.x - textSize.x) / 2.0f;

        graphics.set_draw_color(cen::colors::white.with_alpha(color.alpha()));
        graphics.render_translated_text(name, position + ImVec2{textX, size.y + 4.0f});
      }
    }
  }
}

}  // namespace

void RenderObject(graphics_ctx& graphics,
                  const entt::registry& registry,
                  const entt::entity objectEntity,
                  const cen::color& color)
{
  const auto& object = registry.get<comp::Object>(objectEntity);

  if (!object.visible) {
    return;
  }

  const auto position = ImVec2{object.x, object.y} * graphics.tile_size_ratio();

  switch (object.type) {
    case ObjectType::point:
      RenderPointObject(graphics, registry, objectEntity, position, color);
      break;

    case ObjectType::ellipse:
      RenderEllipseObject(graphics, registry, objectEntity, position, color);
      break;

    case ObjectType::rect:
      RenderRectangleObject(graphics, registry, objectEntity, position, color);
      break;
  }
}

void RenderObjectLayer(graphics_ctx& graphics,
                       const entt::registry& registry,
                       const entt::entity layerEntity,
                       const float parentOpacity)
{
  const auto& layer = registry.get<comp::Layer>(layerEntity);
  const auto& objectLayer = registry.get<comp::ObjectLayer>(layerEntity);

  const auto opacity = parentOpacity * layer.opacity;
  const auto objectColor = cen::color::from_norm(1, 0, 0, opacity);

  for (const auto objectEntity : objectLayer.objects) {
    RenderObject(graphics, registry, objectEntity, objectColor);
  }
}

}  // namespace tactile
