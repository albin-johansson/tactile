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

#include "core/layer/object.hpp"
#include "core/layer/object_layer.hpp"
#include "editor/ui/conversions.hpp"
#include "editor/ui/render/graphics.hpp"
#include "misc/assert.hpp"

namespace tactile::ui {
namespace {

void render_point_object(Graphics& graphics,
                         const Object& object,
                         const ImVec2& position,
                         const uint32 color)
{
  TACTILE_ASSERT(object.is_point());

  const auto& info = graphics.info();
  const float radius = std::min(info.grid_size.x / 4.0f, 6.0f);

  if (graphics.is_within_translated_bounds(position)) {
    graphics.draw_translated_circle_with_shadow(position, radius, color, 2.0f);

    const auto& name = object.ctx().name();
    if (!name.empty()) {
      const auto text_size = ImGui::CalcTextSize(name.c_str());
      if (text_size.x <= info.grid_size.x) {
        const auto text_x = position.x - (text_size.x / 2.0f);
        const auto text_y = position.y + radius + 4.0f;

        graphics.render_translated_text(name.c_str(), {text_x, text_y}, IM_COL32_WHITE);
      }
    }
  }
}

void render_ellipse_object(Graphics& graphics,
                           const Object& object,
                           const ImVec2& position,
                           const uint32 color)
{
  TACTILE_ASSERT(object.is_ellipse());

  const auto& info = graphics.info();
  const auto size = from_vec(object.size());

  const auto radius = ImVec2 {0.5f, 0.5f} * size * info.ratio;
  const auto center = position + radius;

  graphics.draw_translated_ellipse_with_shadow(center, radius, color, 2.0f);

  const auto& name = object.ctx().name();
  if (!name.empty()) {
    const auto text_size = ImGui::CalcTextSize(name.c_str());
    if (text_size.x <= radius.x) {
      const auto text_x = center.x - (text_size.x / 2.0f);
      const auto text_y = center.y + (text_size.y / 2.0f) + (radius.y);

      graphics.render_translated_text(name.c_str(), {text_x, text_y}, IM_COL32_WHITE);
    }
  }
}

void render_rectangle_object(Graphics& graphics,
                             const Object& object,
                             const ImVec2& position,
                             const uint32 color)
{
  TACTILE_ASSERT(object.is_rect());

  const auto& info = graphics.info();
  const auto size = from_vec(object.size()) * info.ratio;

  if (graphics.is_intersecting_bounds(position, size)) {
    graphics.draw_translated_rect_with_shadow(position, size, color, 2.0f);

    const auto& name = object.ctx().name();
    if (!name.empty()) {
      const auto text_size = ImGui::CalcTextSize(name.c_str());
      if (text_size.x <= size.x) {
        const auto text_x = (size.x - text_size.x) / 2.0f;

        graphics.render_translated_text(name.c_str(),
                                        position + ImVec2 {text_x, size.y + 4.0f},
                                        IM_COL32_WHITE);
      }
    }
  }
}

}  // namespace

void render_object(Graphics& graphics, const Object& object, const uint32 color)
{
  if (!object.visible()) {
    return;
  }

  const auto& info = graphics.info();
  const auto position = from_vec(object.pos()) * info.ratio;

  switch (object.type()) {
    case ObjectType::Point:
      render_point_object(graphics, object, position, color);
      break;

    case ObjectType::Ellipse:
      render_ellipse_object(graphics, object, position, color);
      break;

    case ObjectType::Rect:
      render_rectangle_object(graphics, object, position, color);
      break;
  }
}

void render_object_layer(Graphics& graphics,
                         const ObjectLayer& layer,
                         const float parent_opacity)
{
  const auto opacity = parent_opacity * layer.opacity();
  const auto object_color = IM_COL32(0xFF, 0, 0, static_cast<uint8>(255 * opacity));

  for (const auto& [id, object]: layer) {
    render_object(graphics, *object, object_color);
  }
}

}  // namespace tactile::ui
