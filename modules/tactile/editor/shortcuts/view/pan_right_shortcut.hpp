#pragma once

#include "tactile/editor/shortcuts/shortcut.hpp"

namespace Tactile {

class PanRightShortcut final : public AShortcut
{
 public:
  PanRightShortcut();

  void Activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto IsEnabled(const Model& model) const -> bool override;
};

}  // namespace Tactile
