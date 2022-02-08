#pragma once

#include "dialog.hpp"

namespace tactile {

class AboutDialog final : public dialog_base
{
 public:
  AboutDialog();

  void Open();

 protected:
  void on_update(const Model& model, entt::dispatcher& dispatcher) override;
};

}  // namespace tactile
