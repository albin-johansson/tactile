// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/tile/tile_animation.hpp"

#include <cassert>  // assert

#include "tactile/core/world.hpp"
#include "tactile/core/world_data.hpp"

namespace tactile {

void update(CTileAnimation& animation, const SteadyClock::time_point now)
{
  assert(animation.frame_index < animation.frames.size());
  const auto& current_frame = animation.frames.at(animation.frame_index);

  if (now - animation.last_update >= current_frame.duration) {
    const auto next_index = (animation.frame_index + 1) % animation.frames.size();

    animation.last_update = now;
    animation.frame_index = next_index;
  }
}

void update_tile_animations(World& world)
{
  auto& world_data = world.get_data();
  auto& registry = world_data.get_registry();

  const auto now = SteadyClock::now();

  for (auto [_, animation] : registry.view<CTileAnimation>().each()) {
    update(animation, now);
  }
}

}  // namespace tactile
