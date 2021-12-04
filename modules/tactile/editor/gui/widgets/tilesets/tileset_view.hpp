#pragma once

#include <tactile_def.hpp>

#include <entt/entt.hpp>  // registry, entity, dispatcher

namespace Tactile {

class TilesetView final {
 public:
  void Update(const entt::registry& registry,
              entt::entity entity,
              entt::dispatcher& dispatcher);

  [[nodiscard]] auto GetWidth() const -> Maybe<float>;

  [[nodiscard]] auto GetHeight() const -> Maybe<float>;

 private:
  Maybe<float> mWidth;
  Maybe<float> mHeight;
};

}  // namespace Tactile
