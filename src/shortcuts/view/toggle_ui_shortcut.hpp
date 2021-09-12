#pragma once

#include "shortcuts/shortcut.hpp"

namespace Tactile {

class ToggleUiShortcut final : public AShortcut
{
 public:
  ToggleUiShortcut();

  void Activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto IsEnabled(const Model& model) const -> bool override;
};

}  // namespace Tactile
