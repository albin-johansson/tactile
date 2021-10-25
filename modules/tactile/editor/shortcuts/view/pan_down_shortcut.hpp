#pragma once

#include "editor/shortcuts/shortcut.hpp"

namespace Tactile {

class PanDownShortcut final : public AShortcut
{
 public:
  PanDownShortcut();

  void Activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto IsEnabled(const Model& model) const -> bool override;
};

}  // namespace Tactile
