// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/ui/viewport.hpp"

#include "tactile/base/numeric/vec_common.hpp"
#include "tactile/core/debug/assert.hpp"
#include "tactile/core/entity/registry.hpp"

namespace tactile {
namespace {

inline constexpr float kZoomPercentage = 0.05f;

void _clamp_viewport_position(CViewport& viewport, const CViewportLimits& limits)
{
  viewport.pos = max(viewport.pos, limits.min_pos);
  viewport.pos = min(viewport.pos, limits.max_pos);
}

void _scale_viewport_zoom(CViewport& viewport,
                          const CViewportLimits* limits,
                          const Float2& anchor_screen_pos,
                          const float scale_factor)
{
  const auto anchor_world_pos = to_world_pos(viewport, anchor_screen_pos);
  const auto anchor_world_pos_std = anchor_world_pos / viewport.scale;

  viewport.scale *= scale_factor;
  viewport.pos = (anchor_world_pos_std * viewport.scale) - anchor_screen_pos;

  if (limits != nullptr) {
    _clamp_viewport_position(viewport, *limits);
  }
}

}  // namespace

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
  auto& viewport = registry.get<CViewport>(viewport_entity);

  auto& limits = registry.add<CViewportLimits>(viewport_entity);
  limits.min_pos = min_position;
  limits.max_pos = max_position;

  _clamp_viewport_position(viewport, limits);
}

void translate_viewport(CViewport& viewport,
                        const Float2& delta,
                        const CViewportLimits* limits)
{
  viewport.pos += delta;

  if (limits != nullptr) {
    _clamp_viewport_position(viewport, *limits);
  }
}

void increase_viewport_zoom(CViewport& viewport,
                            const Float2& anchor_screen_pos,
                            const CViewportLimits* limits)
{
  _scale_viewport_zoom(viewport, limits, anchor_screen_pos, 1.0f + kZoomPercentage);
}

void decrease_viewport_zoom(CViewport& viewport,
                            const Float2& anchor_screen_pos,
                            const CViewportLimits* limits)
{
  _scale_viewport_zoom(viewport, limits, anchor_screen_pos, 1.0f - kZoomPercentage);
}

void center_viewport_over_content(CViewport& viewport,
                                  const Float2& content_size,
                                  const CViewportLimits* limits)
{
  viewport.pos = (content_size - viewport.size) * 0.5f;

  if (limits != nullptr) {
    _clamp_viewport_position(viewport, *limits);
  }
}

auto to_world_pos(const CViewport& viewport, const Float2& screen_pos) noexcept -> Float2
{
  return viewport.pos + screen_pos;
}

}  // namespace tactile
