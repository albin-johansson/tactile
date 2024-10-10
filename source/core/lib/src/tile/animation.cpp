// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/tile/animation.hpp"

#include "tactile/base/numeric/saturate_cast.hpp"
#include "tactile/core/debug/assert.hpp"
#include "tactile/core/entity/registry.hpp"
#include "tactile/core/tile/tile.hpp"

namespace tactile::core {
namespace {

void _reset_animation(CAnimation& animation)
{
  animation.last_update = SteadyClock::now();
  animation.frame_index = 0;
}

}  // namespace

void update_animations(Registry& registry)
{
  const auto now = SteadyClock::now();

  for (auto [entity, animation] : registry.each<CAnimation>()) {
    const auto& current_frame = animation.frames.at(animation.frame_index);

    if ((now - animation.last_update) >= current_frame.duration) {
      const auto next_index = (animation.frame_index + 1) % animation.frames.size();

      animation.last_update = now;
      animation.frame_index = next_index;
    }
  }
}

auto add_animation_frame(Registry& registry,
                         const EntityID tile_entity,
                         const std::size_t frame_index,
                         const AnimationFrame& frame) -> std::expected<void, ErrorCode>
{
  TACTILE_ASSERT(is_tile(registry, tile_entity));
  const auto is_animated = registry.has<CAnimation>(tile_entity);

  if (!is_animated && frame_index != 0) {
    return std::unexpected {ErrorCode::kBadParam};
  }

  if (!is_animated) {
    registry.add<CAnimation>(tile_entity);
  }

  auto& animation = registry.get<CAnimation>(tile_entity);
  const auto frame_count = animation.frames.size();

  if (frame_index == frame_count) {
    animation.frames.push_back(frame);
  }
  else if (frame_index < frame_count) {
    const auto pos = animation.frames.begin() + saturate_cast<std::ptrdiff_t>(frame_index);
    animation.frames.insert(pos, frame);
  }
  else {
    return std::unexpected {ErrorCode::kBadParam};
  }

  _reset_animation(animation);

  return {};
}

auto remove_animation_frame(Registry& registry,
                            const EntityID tile_entity,
                            const std::size_t frame_index) -> std::expected<void, ErrorCode>
{
  TACTILE_ASSERT(is_tile(registry, tile_entity));
  TACTILE_ASSERT(registry.has<CAnimation>(tile_entity));

  auto& animation = registry.get<CAnimation>(tile_entity);

  if (frame_index >= animation.frames.size()) {
    return std::unexpected {ErrorCode::kBadParam};
  }

  const auto pos = animation.frames.begin() + saturate_cast<std::ptrdiff_t>(frame_index);
  animation.frames.erase(pos);

  if (animation.frames.empty()) {
    registry.erase<CAnimation>(tile_entity);
  }
  else {
    _reset_animation(animation);
  }

  return {};
}

}  // namespace tactile::core
