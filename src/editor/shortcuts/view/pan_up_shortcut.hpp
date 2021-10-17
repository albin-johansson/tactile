#pragma once

#include "editor/shortcuts/shortcut.hpp"

namespace Tactile {

class PanUpShortcut final : public AShortcut
{
 public:
  PanUpShortcut();

  void Activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto IsEnabled(const Model& model) const -> bool override;
};

}  // namespace Tactile
