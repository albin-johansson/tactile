#pragma once

#include "dialog.hpp"

namespace Tactile {

class CreditsDialog final : public ADialog {
 public:
  CreditsDialog();

  void Open();

 protected:
  void UpdateContents(const Model& model, entt::dispatcher& dispatcher) override;
};

}  // namespace Tactile
