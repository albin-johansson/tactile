#pragma once

#include "component_name_dialog.hpp"

namespace Tactile {

class CreateComponentDialog final : public AComponentNameDialog {
 public:
  CreateComponentDialog();

  void Open();

 protected:
  void OnAccept(entt::dispatcher& dispatcher) override;
};

}  // namespace Tactile