#pragma once

#include "dialog.hpp"

namespace tactile {

class AboutDialog final : public ADialog
{
 public:
  AboutDialog();

  void Open();

 protected:
  void UpdateContents(const Model& model, entt::dispatcher& dispatcher) override;
};

}  // namespace tactile
