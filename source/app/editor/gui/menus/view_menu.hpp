#pragma once

#include <entt/entt.hpp>

namespace tactile {

class document_model;

class ViewMenu final
{
 public:
  void Update(const document_model& model, entt::dispatcher& dispatcher);
};

}  // namespace tactile
