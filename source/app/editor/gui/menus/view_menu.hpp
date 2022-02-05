#pragma once

#include <entt/entt.hpp>

namespace tactile {

class Model;

class ViewMenu final
{
 public:
  void Update(const Model& model, entt::dispatcher& dispatcher);
};

}  // namespace tactile
