#pragma once

#include "shortcuts/shortcut.hpp"

namespace Tactile {

class DecreaseViewportZoomShortcut final : public AShortcut
{
 public:
  DecreaseViewportZoomShortcut();

  void Activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto IsEnabled(const Model& model) const -> bool override;
};

}  // namespace Tactile