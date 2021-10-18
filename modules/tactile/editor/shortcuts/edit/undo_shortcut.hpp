#pragma once

#include "tactile/editor/shortcuts/shortcut.hpp"

namespace Tactile {

class UndoShortcut final : public AShortcut
{
 public:
  UndoShortcut();

  void Activate(entt::dispatcher& dispatcher) override;

  [[nodiscard]] auto IsEnabled(const Model& model) const -> bool override;
};

}  // namespace Tactile
