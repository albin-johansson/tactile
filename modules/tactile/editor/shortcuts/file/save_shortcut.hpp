#pragma once

#include "tactile/editor/shortcuts/shortcut.hpp"

namespace Tactile {

class SaveShortcut final : public AShortcut
{
 public:
  SaveShortcut();

  void Activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto IsEnabled(const Model& model) const -> bool override;
};

}  // namespace Tactile
