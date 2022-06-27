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

#include <centurion/color.hpp>
#include <entt/entity/registry.hpp>
#include <imgui.h>
#include <imgui_internal.h>

#include "core/common/ecs.hpp"
#include "core/components/attributes.hpp"
#include "core/components/layers.hpp"
#include "core/components/objects.hpp"
#include "core/layers/object.hpp"
#include "core/layers/object_layer.hpp"
#include "editor/ui/rendering/graphics.hpp"
#include "misc/assert.hpp"

namespace tactile::ui {
namespace {

void _render_point_object(GraphicsCtx&        graphics,
                          const core::Object& object,
                          const ImVec2&       position,
                          const cen::color&   color)
{
  TACTILE_ASSERT(object.get_type() == ObjectType::Point);

  const float radius = (std::min)(graphics.viewport_tile_size().x / 4.0f, 6.0f);

  if (graphics.is_within_translated_bounds(position)) {
    graphics.set_draw_color(color);
    graphics.set_line_thickness(2.0f);
    graphics.draw_translated_circle_with_shadow(position, radius);

    const auto& name = object.get_name();
    if (!name.empty()) {
      const auto textSize = ImGui::CalcTextSize(name.c_str());
      if (textSize.x <= graphics.viewport_tile_size().x) {
        const auto textX = position.x - (textSize.x / 2.0f);
        const auto textY = position.y + radius + 4.0f;

        graphics.set_draw_color(cen::colors::white);
        graphics.render_translated_text(name.c_str(), {textX, textY});
      }
    }
  }
}

void _render_ellipse_object(GraphicsCtx&        graphics,
                            const core::Object& object,
                            const ImVec2&       position,
                            const cen::color&   color)
{
  TACTILE_ASSERT(object.get_type() == ObjectType::Ellipse);

  const ImVec2 size = {object.get_size().x, object.get_size().y};

  const auto radius = ImVec2{0.5f, 0.5f} * size * graphics.tile_size_ratio();
  const auto center = position + radius;

  graphics.set_draw_color(color);
  graphics.set_line_thickness(2);
  graphics.draw_translated_ellipse_with_shadow(center, radius);

  const auto& name = object.get_name();
  if (!name.empty()) {
    const auto textSize = ImGui::CalcTextSize(name.c_str());
    if (textSize.x <= radius.x) {
      const auto textX = center.x - (textSize.x / 2.0f);
      const auto textY = center.y + (textSize.y / 2.0f) + (radius.y);

      graphics.set_draw_color(cen::colors::white);
      graphics.render_translated_text(name.c_str(), {textX, textY});
    }
  }
}

void _render_rectangle_object(GraphicsCtx&        graphics,
                              const core::Object& object,
                              const ImVec2&       position,
                              const cen::color&   color)
{
  TACTILE_ASSERT(object.get_type() == ObjectType::Rect);

  const auto size =
      ImVec2{object.get_size().x, object.get_size().y} * graphics.tile_size_ratio();

  if (graphics.is_intersecting_bounds(position, size)) {
    graphics.set_draw_color(color);
    graphics.set_line_thickness(2.0f);
    graphics.draw_translated_rect_with_shadow(position, size);

    const auto& name = object.get_name();
    if (!name.empty()) {
      const auto textSize = ImGui::CalcTextSize(name.c_str());
      if (textSize.x <= size.x) {
        const auto textX = (size.x - textSize.x) / 2.0f;

        graphics.set_draw_color(cen::colors::white.with_alpha(color.alpha()));
        graphics.render_translated_text(name.c_str(),
                                        position + ImVec2{textX, size.y + 4.0f});
      }
    }
  }
}

}  // namespace

void render_object(GraphicsCtx&        graphics,
                   const core::Object& object,
                   const cen::color&   color)
{
  if (!object.is_visible()) {
    return;
  }

  const auto position =
      ImVec2{object.get_pos().x, object.get_pos().y} * graphics.tile_size_ratio();

  switch (object.get_type()) {
    case ObjectType::Point:
      _render_point_object(graphics, object, position, color);
      break;

    case ObjectType::Ellipse:
      _render_ellipse_object(graphics, object, position, color);
      break;

    case ObjectType::Rect:
      _render_rectangle_object(graphics, object, position, color);
      break;
  }
}

void render_object_layer(GraphicsCtx&             graphics,
                         const core::ObjectLayer& layer,
                         const float              parentOpacity)
{
  const auto opacity = parentOpacity * layer.get_opacity();
  const auto objectColor = cen::color::from_norm(1, 0, 0, opacity);

  for (const auto& [id, object] : layer) {
    render_object(graphics, object, objectColor);
  }
}

}  // namespace tactile::ui
