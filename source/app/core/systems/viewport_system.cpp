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

#include "viewport_system.hpp"

#include <algorithm>  // min, max
#include <cmath>      // round
#include <utility>    // pair

#include <entt/entity/registry.hpp>

#include "core/common/ecs.hpp"
#include "core/components/map_info.hpp"
#include "core/components/viewport.hpp"
#include "misc/assert.hpp"

namespace tactile::sys {
namespace {

constexpr float _min_tile_height = 4;

[[nodiscard]] auto _get_viewport_offset_delta(const float tileWidth, const float ratio)
    -> std::pair<float, float>
{
  const auto dx = std::round((std::max)(2.0f, tileWidth * 0.05f));
  const auto dy = dx / ratio;

  return {dx, dy};
}

}  // namespace

void offset_viewport(entt::registry& registry, const float dx, const float dy)
{
  auto& viewport = ctx_get<comp::Viewport>(registry);
  viewport.offset.x += dx;
  viewport.offset.y += dy;
}

void offset_viewport(entt::registry& registry,
                     const entt::entity entity,
                     const Vector2f& offset)
{
  auto& viewport = entity != entt::null ? checked_get<comp::Viewport>(registry, entity)
                                        : ctx_get<comp::Viewport>(registry);

  viewport.offset += offset;

  if (entity != entt::null) {
    if (auto* limits = registry.try_get<comp::ViewportLimits>(entity)) {
      viewport.offset.x = (std::min)(limits->min_offset.x, viewport.offset.x);
      viewport.offset.y = (std::min)(limits->min_offset.y, viewport.offset.y);

      viewport.offset.x = (std::max)(limits->max_offset.x, viewport.offset.x);
      viewport.offset.y = (std::max)(limits->max_offset.y, viewport.offset.y);
    }
  }
}

void pan_viewport_left(entt::registry& registry)
{
  auto& viewport = ctx_get<comp::Viewport>(registry);
  viewport.offset.x += viewport.tile_size.x;
}

void pan_viewport_right(entt::registry& registry)
{
  auto& viewport = ctx_get<comp::Viewport>(registry);
  viewport.offset.x -= viewport.tile_size.x;
}

void pan_viewport_up(entt::registry& registry)
{
  auto& viewport = ctx_get<comp::Viewport>(registry);
  viewport.offset.y += viewport.tile_size.y;
}

void pan_viewport_down(entt::registry& registry)
{
  auto& viewport = ctx_get<comp::Viewport>(registry);
  viewport.offset.y -= viewport.tile_size.y;
}

void reset_viewport_zoom(entt::registry& registry)
{
  const auto& map = ctx_get<comp::MapInfo>(registry);
  auto& viewport = ctx_get<comp::Viewport>(registry);

  viewport.tile_size.x = 2.0f * static_cast<float>(map.tile_size.x);
  viewport.tile_size.y = 2.0f * static_cast<float>(map.tile_size.y);
}

void decrease_viewport_zoom(entt::registry& registry,
                            const float mouseX,
                            const float mouseY)
{
  TACTILE_ASSERT(can_decrease_viewport_zoom(registry));

  auto& viewport = ctx_get<comp::Viewport>(registry);

  // Percentages of map to the left of and above the cursor
  const auto px = (mouseX - viewport.offset.x) / viewport.tile_size.x;
  const auto py = (mouseY - viewport.offset.y) / viewport.tile_size.y;

  {
    const auto ratio = viewport.tile_size.x / viewport.tile_size.y;
    const auto [dx, dy] = _get_viewport_offset_delta(viewport.tile_size.x, ratio);
    viewport.tile_size.x -= dx;
    viewport.tile_size.y -= dy;

    viewport.tile_size.x = (std::max)(_min_tile_height * ratio, viewport.tile_size.x);
    viewport.tile_size.y = (std::max)(_min_tile_height, viewport.tile_size.y);
  }

  viewport.offset.x = mouseX - (px * viewport.tile_size.x);
  viewport.offset.y = mouseY - (py * viewport.tile_size.y);
}

void increase_viewport_zoom(entt::registry& registry,
                            const float mouseX,
                            const float mouseY)
{
  auto& viewport = ctx_get<comp::Viewport>(registry);

  // Percentages of map to the left of and above the cursor
  const auto px = (mouseX - viewport.offset.x) / viewport.tile_size.x;
  const auto py = (mouseY - viewport.offset.y) / viewport.tile_size.y;

  {
    const auto ratio = viewport.tile_size.x / viewport.tile_size.y;
    const auto [dx, dy] = _get_viewport_offset_delta(viewport.tile_size.x, ratio);
    viewport.tile_size.x += dx;
    viewport.tile_size.y += dy;
  }

  viewport.offset.x = mouseX - (px * viewport.tile_size.x);
  viewport.offset.y = mouseY - (py * viewport.tile_size.y);
}

auto can_decrease_viewport_zoom(const entt::registry& registry) -> bool
{
  const auto& viewport = ctx_get<comp::Viewport>(registry);
  return viewport.tile_size.y > _min_tile_height;
}

auto get_viewport_scaling_ratio(const entt::registry& registry) -> ViewportScalingRatio
{
  const auto& viewport = ctx_get<comp::Viewport>(registry);
  const auto& map = ctx_get<comp::MapInfo>(registry);
  const auto ratio = viewport.tile_size / Vector2f{map.tile_size};
  return {ratio.x, ratio.y};
}

}  // namespace tactile::sys
