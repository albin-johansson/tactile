#pragma once

#include "component_name_dialog.hpp"

namespace tactile {

class CreateComponentDialog final : public AComponentNameDialog
{
 public:
  CreateComponentDialog();

  void Open();

 protected:
  void on_accept(entt::dispatcher& dispatcher) override;
};

}  // namespace tactile