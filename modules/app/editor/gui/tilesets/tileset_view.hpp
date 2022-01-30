#pragma once

#include <entt/entt.hpp>

#include "tactile_def.hpp"

namespace tactile {

class TilesetView final {
 public:
  void Update(const entt::registry& registry,
              entt::entity entity,
              entt::dispatcher& dispatcher);

  [[nodiscard]] auto GetWidth() const -> maybe<float>;

  [[nodiscard]] auto GetHeight() const -> maybe<float>;

 private:
  maybe<float> mWidth;
  maybe<float> mHeight;
};

}  // namespace tactile
