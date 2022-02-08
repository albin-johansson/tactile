#pragma once

#include "dialog.hpp"

namespace tactile {

class CreditsDialog final : public dialog_base
{
 public:
  CreditsDialog();

  void Open();

 protected:
  void on_update(const Model& model, entt::dispatcher& dispatcher) override;
};

}  // namespace tactile
