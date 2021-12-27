#include "animation_system.hpp"

#include <centurion.hpp>  // ticks

#include "core/components/animation.hpp"

namespace Tactile::Sys {

void UpdateAnimations(entt::registry& registry)
{
  for (auto&& [entity, animation] : registry.view<Animation>().each()) {
    const auto now = cen::GetTicks();

    const auto frameEntity = animation.frames.at(animation.index);
    const auto& currentFrame = registry.get<AnimationFrame>(frameEntity);

    if (now - animation.last_update_time >= currentFrame.duration) {
      animation.last_update_time = now;
      animation.index = (animation.index + 1) % animation.frames.size();
    }
  }
}

}  // namespace Tactile::Sys
