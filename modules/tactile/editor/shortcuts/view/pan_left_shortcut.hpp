#pragma once

#include "tactile/editor/shortcuts/shortcut.hpp"

namespace Tactile {

class PanLeftShortcut final : public AShortcut
{
 public:
  PanLeftShortcut();

  void Activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto IsEnabled(const Model& model) const -> bool override;
};

}  // namespace Tactile
