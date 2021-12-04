#pragma once

#include <entt/entt.hpp>

namespace Tactile {

class Model;

class ViewMenu final {
 public:
  void Update(const Model& model, entt::dispatcher& dispatcher);

 private:
  void UpdateWidgetsMenu(bool hasActiveMap);
};

}  // namespace Tactile
