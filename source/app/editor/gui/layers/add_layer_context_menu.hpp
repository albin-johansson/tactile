#pragma once

#include "editor/gui/common/context_menu.hpp"

namespace tactile {

class add_layer_context_menu final : public context_menu
{
 public:
  add_layer_context_menu();

 protected:
  void on_update(const document_model& model, entt::dispatcher& dispatcher) override;
};

}  // namespace tactile
