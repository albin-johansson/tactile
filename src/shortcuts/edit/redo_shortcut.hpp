#pragma once

#include "shortcuts/shortcut.hpp"

namespace Tactile {

class RedoShortcut final : public AShortcut
{
 public:
  RedoShortcut();

  void Activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto IsEnabled(const Model& model) const -> bool override;
};

}  // namespace Tactile
