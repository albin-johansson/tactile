#include "viewport_system.hpp"

#include <algorithm>  // min, max
#include <cmath>      // round
#include <utility>    // pair

#include "assert.hpp"
#include "core/components/texture.hpp"
#include "core/map.hpp"
#include "core/mouse_pos.hpp"
#include "core/viewport.hpp"

namespace tactile::Sys {
namespace {

constexpr float gMinTileHeight = 4;

[[nodiscard]] auto GetViewportOffsetDelta(const float tileWidth, const float ratio)
    -> std::pair<float, float>
{
  const auto dx = std::round(std::max(2.0f, tileWidth * 0.05f));
  const auto dy = dx / ratio;

  return {dx, dy};
}

}  // namespace

void OffsetViewport(entt::registry& registry, const float dx, const float dy)
{
  auto& viewport = registry.ctx<Viewport>();
  viewport.x_offset += dx;
  viewport.y_offset += dy;
}

void OffsetBoundViewport(entt::registry& registry,
                         const entt::entity entity,
                         const float dx,
                         const float dy,
                         const float viewWidth,
                         const float viewHeight)
{
  TACTILE_ASSERT(entity != entt::null);
  TACTILE_ASSERT(registry.all_of<Viewport>(entity));
  TACTILE_ASSERT(registry.all_of<Texture>(entity));

  auto& viewport = registry.get<Viewport>(entity);
  viewport.x_offset += dx;
  viewport.y_offset += dy;

  viewport.x_offset = std::min(0.0f, viewport.x_offset);
  viewport.y_offset = std::min(0.0f, viewport.y_offset);

  const auto& texture = registry.get<Texture>(entity);
  const auto textureWidth = static_cast<float>(texture.width);
  const auto textureHeight = static_cast<float>(texture.height);

  viewport.x_offset = std::max(-textureWidth + viewWidth, viewport.x_offset);
  viewport.y_offset = std::max(-textureHeight + viewHeight, viewport.y_offset);
}

void PanViewportLeft(entt::registry& registry)
{
  auto& viewport = registry.ctx<Viewport>();
  viewport.x_offset += viewport.tile_width;
}

void PanViewportRight(entt::registry& registry)
{
  auto& viewport = registry.ctx<Viewport>();
  viewport.x_offset -= viewport.tile_width;
}

void PanViewportUp(entt::registry& registry)
{
  auto& viewport = registry.ctx<Viewport>();
  viewport.y_offset += viewport.tile_height;
}

void PanViewportDown(entt::registry& registry)
{
  auto& viewport = registry.ctx<Viewport>();
  viewport.y_offset -= viewport.tile_height;
}

void ResetViewportZoom(entt::registry& registry)
{
  const auto& map = registry.ctx<Map>();
  auto& viewport = registry.ctx<Viewport>();
  viewport.tile_width = 2.0f * static_cast<float>(map.tile_width);
  viewport.tile_height = 2.0f * static_cast<float>(map.tile_height);
}

void DecreaseViewportZoom(entt::registry& registry)
{
  TACTILE_ASSERT(CanDecreaseViewportZoom(registry));

  auto& viewport = registry.ctx<Viewport>();
  const auto& mouse = registry.ctx<MousePos>();

  // Percentages of map to the left of and above the cursor
  const auto px = (mouse.x - viewport.x_offset) / viewport.tile_width;
  const auto py = (mouse.y - viewport.y_offset) / viewport.tile_height;

  {
    const auto ratio = viewport.tile_width / viewport.tile_height;
    const auto [dx, dy] = GetViewportOffsetDelta(viewport.tile_width, ratio);
    viewport.tile_width -= dx;
    viewport.tile_height -= dy;

    viewport.tile_width = std::max(gMinTileHeight * ratio, viewport.tile_width);
    viewport.tile_height = std::max(gMinTileHeight, viewport.tile_height);
  }

  viewport.x_offset = mouse.x - (px * viewport.tile_width);
  viewport.y_offset = mouse.y - (py * viewport.tile_height);
}

void IncreaseViewportZoom(entt::registry& registry)
{
  auto& viewport = registry.ctx<Viewport>();
  const auto& mouse = registry.ctx<MousePos>();

  // Percentages of map to the left of and above the cursor
  const auto px = (mouse.x - viewport.x_offset) / viewport.tile_width;
  const auto py = (mouse.y - viewport.y_offset) / viewport.tile_height;

  {
    const auto ratio = viewport.tile_width / viewport.tile_height;
    const auto [dx, dy] = GetViewportOffsetDelta(viewport.tile_width, ratio);
    viewport.tile_width += dx;
    viewport.tile_height += dy;
  }

  viewport.x_offset = mouse.x - (px * viewport.tile_width);
  viewport.y_offset = mouse.y - (py * viewport.tile_height);
}

auto CanDecreaseViewportZoom(const entt::registry& registry) -> bool
{
  const auto& viewport = registry.ctx<Viewport>();
  return viewport.tile_height > gMinTileHeight;
}

auto GetViewportScalingRatio(const entt::registry& registry) -> ViewportScalingRatio
{
  const auto& viewport = registry.ctx<Viewport>();
  const auto& map = registry.ctx<Map>();

  const auto xRatio = viewport.tile_width / static_cast<float>(map.tile_width);
  const auto yRatio = viewport.tile_height / static_cast<float>(map.tile_height);

  return {xRatio, yRatio};
}

}  // namespace tactile::Sys
