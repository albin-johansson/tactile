// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/viewport.hpp"

#include "tactile/core/debug/assert.hpp"
#include "tactile/core/entity/registry.hpp"
#include "tactile/core/numeric/vec_common.hpp"

namespace tactile {
inline namespace viewport {

inline constexpr float kZoomPercentage = 0.05f;

void _clamp_viewport_position(Registry& registry,
                              const EntityID viewport_entity,
                              CViewport& viewport)
{
  if (const auto* limits = registry.find<CViewportLimits>(viewport_entity)) {
    viewport.pos = max(viewport.pos, limits->min_pos);
    viewport.pos = min(viewport.pos, limits->max_pos);
  }
}

void _scale_viewport_zoom(Registry& registry,
                          const EntityID viewport_entity,
                          const Float2& anchor_screen_pos,
                          const float scale_factor)
{
  TACTILE_ASSERT(is_viewport(registry, viewport_entity));
  auto& viewport = registry.get<CViewport>(viewport_entity);

  const auto anchor_world_pos = to_world_pos(viewport, anchor_screen_pos);
  const auto anchor_world_pos_std = anchor_world_pos / viewport.scale;

  viewport.scale *= scale_factor;
  viewport.pos = (anchor_world_pos_std * viewport.scale) - anchor_screen_pos;

  _clamp_viewport_position(registry, viewport_entity, viewport);
}

}  // namespace viewport

auto is_viewport(const Registry& registry, const EntityID entity) -> bool
{
  return registry.has<CViewport>(entity);
}

void set_viewport_limits(Registry& registry,
                         const EntityID viewport_entity,
                         const Float2& min_position,
                         const Float2& max_position)
{
  TACTILE_ASSERT(is_viewport(registry, viewport_entity));

  auto& limits = registry.add<CViewportLimits>(viewport_entity);
  limits.min_pos = min_position;
  limits.max_pos = max_position;

  auto& viewport = registry.get<CViewport>(viewport_entity);
  _clamp_viewport_position(registry, viewport_entity, viewport);
}

void translate_viewport(Registry& registry,
                        const EntityID viewport_entity,
                        const Float2& delta)
{
  TACTILE_ASSERT(is_viewport(registry, viewport_entity));

  auto& viewport = registry.get<CViewport>(viewport_entity);
  viewport.pos += delta;

  _clamp_viewport_position(registry, viewport_entity, viewport);
}

void increase_viewport_zoom(Registry& registry,
                            const EntityID viewport_entity,
                            const Float2& anchor_screen_pos)
{
  _scale_viewport_zoom(registry,
                       viewport_entity,
                       anchor_screen_pos,
                       1.0f + kZoomPercentage);
}

void decrease_viewport_zoom(Registry& registry,
                            const EntityID viewport_entity,
                            const Float2& anchor_screen_pos)
{
  _scale_viewport_zoom(registry,
                       viewport_entity,
                       anchor_screen_pos,
                       1.0f - kZoomPercentage);
}

void reset_viewport_zoom(Registry& registry, const EntityID viewport_entity)
{
  TACTILE_ASSERT(is_viewport(registry, viewport_entity));

  auto& viewport = registry.get<CViewport>(viewport_entity);
  viewport.scale = 1.0f;
}

void center_viewport_over_content(Registry& registry,
                                  const EntityID viewport_entity,
                                  const Float2& content_size)
{
  TACTILE_ASSERT(is_viewport(registry, viewport_entity));

  auto& viewport = registry.get<CViewport>(viewport_entity);
  viewport.pos = ((content_size - viewport.size) * 0.5f);

  _clamp_viewport_position(registry, viewport_entity, viewport);
}

auto to_world_pos(const CViewport& viewport,
                  const Float2& screen_pos) noexcept -> Float2
{
  return viewport.pos + screen_pos;
}

}  // namespace tactile
