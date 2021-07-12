#pragma once

#include "shortcuts/shortcut.hpp"

namespace Tactile {

class IncreaseViewportZoomShortcut final : public AShortcut
{
 public:
  IncreaseViewportZoomShortcut();

  void Activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto IsEnabled(const Model& model) const -> bool override;
};

}  // namespace Tactile
