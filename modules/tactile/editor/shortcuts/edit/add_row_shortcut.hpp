#pragma once

#include "editor/shortcuts/shortcut.hpp"

namespace Tactile {

class AddRowShortcut final : public AShortcut
{
 public:
  AddRowShortcut();

  void Activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto IsEnabled(const Model& model) const -> bool override;
};

}  // namespace Tactile
