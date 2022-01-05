#pragma once

#include <entt/entt.hpp>

namespace Tactile {

class ComponentDock final {
 public:
  void Update(const entt::registry& registry, entt::dispatcher& dispatcher);

 private:
  bool mHasFocus{};
};

}  // namespace Tactile
