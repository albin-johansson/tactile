#pragma once

#include "shortcuts/shortcut.hpp"

namespace Tactile {

class SaveAsShortcut final : public AShortcut
{
 public:
  SaveAsShortcut();

  void Activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto IsEnabled(const Model& model) const -> bool override;
};

}  // namespace Tactile
